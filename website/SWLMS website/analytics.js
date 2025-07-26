// Ive nt tested all the functions  lol. But you  do slight edits 
const API_KEY = 'L6EE5X3VM6Z3N5ZV';
const CHANNEL_ID = '3001129';
const BASE_URL = `https://api.thingspeak.com/channels/${CHANNEL_ID}/feeds.json?api_key=${API_KEY}&results=8000`;

// pulls tank data from ThingSpeak
async function fetchData() {
  const res = await fetch(BASE_URL);
  const json = await res.json();
  return json.feeds.map(f => ({
    timestamp: new Date(f.created_at),
    depth: parseFloat(f.field1),
    litres: parseFloat(f.field2),
    percentage: parseFloat(f.field3)
  })).filter(d => !isNaN(d.depth));
}

// stats that tell how full/empty the tank gets
function getLowestHighest(data, key) {
  const vals = data.map(d => d[key]);
  return { min: Math.min(...vals), max: Math.max(...vals) };
}

// average tank level – pretty self-explanatory
function getAverage(data, key) {
  const sum = data.reduce((a, d) => a + d[key], 0);
  return sum / data.length;
}

// standard deviation – for spotting volatility in levels
function getStdDev(data, key) {
  const avg = getAverage(data, key);
  const variance = data.reduce((a, d) => a + Math.pow(d[key] - avg, 2), 0) / data.length;
  return Math.sqrt(variance);
}

// breaks down water usage by day
function groupByDay(data) {
  return data.reduce((acc, d) => {
    const day = d.timestamp.toISOString().split('T')[0];
    acc[day] = acc[day] || [];
    acc[day].push(d);
    return acc;
  }, {});
}

// returns days with highest usage (top 5)
function daysWithHighestUsage(data) {
  const byDay = groupByDay(data);
  const usage = Object.entries(byDay).map(([day, vals]) => {
    const start = vals[0].litres;
    const end = vals[vals.length-1].litres;
    return { day, usage: start - end };
  });
  usage.sort((a, b) => b.usage - a.usage);
  return usage.slice(0, 5);
}

// detects overflows (when level goes above 95%)
function detectOverflows(data, threshold = 95) {
  const events = [];
  let inOverflow = false, start, lost = 0;
  for (let i = 1; i < data.length; i++) {
    if (!inOverflow && data[i].percentage > threshold) {
      inOverflow = true; start = data[i].timestamp; lost = 0;
    }
    if (inOverflow) {
      if (data[i].percentage <= threshold) {
        events.push({ start, end: data[i].timestamp, duration: (data[i].timestamp - start)/1000, lost });
        inOverflow = false;
      } else {
        lost += data[i].litres - data[i-1].litres;
      }
    }
  }
  return events;
}

// estimates how often the pump kicks in per hour
function calculatePumpEfficiency(data) {
  let cycles = 0;
  for (let i = 1; i < data.length; i++) {
    if (data[i-1].litres < data[i].litres && data[i].litres > data[i+1]?.litres) cycles++;
  }
  const durationHrs = (data[data.length-1].timestamp - data[0].timestamp) / 36e5;
  return { cycles, perHour: cycles / durationHrs };
}

// burst pipe detector – flags massive sudden jumps in litres
function detectBurstPipes(data) {
  const avgRefill = getAverage(data.filter((d,i)=>i>0 && d.litres>data[i-1].litres), 'litres');
  return data.reduce((acc, d, i, arr) => {
    if (i>0 && d.litres - arr[i-1].litres > avgRefill * 2) {
      acc.push({ time: d.timestamp, amount: d.litres - arr[i-1].litres });
    }
    return acc;
  }, []);
}

// how many pump cycles in a given duration (e.g. 24h)
function calculateCyclesPerDuration(data, ms) {
  const cutoff = new Date(data[data.length-1].timestamp - ms);
  return calculatePumpEfficiency(data.filter(d => d.timestamp > cutoff));
}

// how many fills happened in a time window (e.g. last day)
function calculateFillFrequency(data, ms) {
  const cutoff = new Date(data[data.length-1].timestamp - ms);
  const fills = data.filter((d,i,arr)=>i>0 && d.litres>arr[i-1].litres && d.timestamp>cutoff);
  return fills.length;
}

// avg time (in seconds) between refill start and stop
function calculateAverageRefillTime(data) {
  let times = [];
  for (let i = 1; i < data.length; i++) {
    if (data[i].litres > data[i-1].litres) {
      times.push((data[i].timestamp - data[i-1].timestamp)/1000);
    }
  }
  return times.reduce((a,b)=>a+b,0)/times.length;
}

// picks the tank that needs relief – placeholder logic
function suggestLoadBalancing(rates) {
  const max = Object.entries(rates).sort((a,b)=>b[1]-a[1])[0][0];
  return `Balance load by routing excess from ${max}`; // because teamwork makes the stream work
}

// gets weather forecast from Open-Meteo (hourly temps)
async function getWeatherForecast() {
  const res = await fetch("https://api.open-meteo.com/v1/forecast?latitude=0.3&longitude=32.6&hourly=temperature_2m&forecast_days=2&timezone=Africa%2FNairobi");
  const json = await res.json();
  return json.hourly.time.map((t, i) => ({ date: t, temp: json.hourly.temperature_2m[i] }));
}

// if temp > 25°C, assume refill will likely be needed – tweak as needed
async function predictRefills(weatherForecast) {
  return weatherForecast.map(w => ({ date: w.date, expectedRefill: w.temp > 25 ? 1 : 0 }));
}

// run everything at once (final output)
(async () => {
  const data = await fetchData();
  const forecast = await getWeatherForecast();
  const result = {
    lowestHighest: getLowestHighest(data, 'percentage'),
    avgLevel: getAverage(data, 'percentage'),
    stdDev: getStdDev(data, 'percentage'),
    topUsageDays: daysWithHighestUsage(data),
    overflows: detectOverflows(data),
    pumpEfficiency: calculatePumpEfficiency(data),
    burstPipes: detectBurstPipes(data),
    cyclesLastDay: calculateCyclesPerDuration(data, 86400000),
    fillsLastDay: calculateFillFrequency(data, 86400000),
    avgRefillTime: calculateAverageRefillTime(data),
    loadBalancing: suggestLoadBalancing({ tankA: 5, tankB: 3 }),
    refillPrediction: await predictRefills(forecast)
  };
  console.log(JSON.stringify(result, null, 2)); // your frontend guy can grab this json
})();
