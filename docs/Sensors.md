# Sensor Information
[←Top](../README.md)<BR>

### Collecting Wind Data
Wind_SampleSpeed() - Return a wind speed based on how many interrupts and duration between calls to this function.
Wind_SampleDirection() - Talk i2c to the AS5600 sensor and get direction.
Wind_TakeReading() - Call this function every second. It calls wind direction and wind speed functions. Then saves samples in a circular buffer of 60 buckets.

### Creating the Wind Observations
Wind_DirectionVector() - Uses the 60 sample buckets of wind direction where wind speed is greater than zero to compute and return a wind vector.
Wind_SpeedAverage() - Uses the 60 sample buckets of wind speed to return a wind speed average.
Wind_GustUpdate() - Uses the most current highest 3 consecutive samples of wind speed from the 60 samples. The 3 samples are then averaged for wind gust.
Variables wind.gust and wind.gust_directionare set by this function.
Call this function before calling Wind_Gust() and Wind_GustDirection().
Wind_Gust() - Returns wind.gust
Wind_GustDirection() - Returns wind.gust_direction


### PM25AQI - I2C - Air Quality Sensor

Data variables returned
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 350px; border: 1px solid black; padding: 10px;">
<pre>

  pm10_standard  < Standard Particle PM1.0 concentration unit µg 𝑚3
  pm25_standard  < Standard Particle PM2.5 concentration unit µg 𝑚3
  pm100_standard < Standard Particle PM10.0 concentration unit µg 𝑚3
  pm10_env        < Atmospheric Environmental PM1.0 concentration unit µg 𝑚3
  pm25_env        < Atmospheric Environmental PM2.5 concentration unit µg 𝑚3
  pm100_env      < Atmospheric Environmental PM10.0 concentration unit µg 𝑚3
  particles_03um < Particles with diameter beyond 0.3 µ 𝑚 in 0.1L of air
  particles_05um < Particles with diameter beyond 0.5 µ 𝑚 in 0.1L of air
  particles_10um < Particles with diameter beyond 1.0 µ 𝑚 in 0.1L of air
  particles_25um < Particles with diameter beyond 2.5 µ 𝑚 in 0.1L of air
  particles_50um < Particles with diameter beyond 5.0 µ 𝑚 in 0.1L of air
  particles_100um   < Particles with diameter beyond 10.0 µ 𝑚 in 0.1L of air

  pms = Particulate Matter Standard
  pme = Particulate Matter Environmental

  Variable Tags for what we monitor and report on
  pm1s10
  pm1s25
  pm1s100
  pm1e10
  pm1e25
  pm1e100
</pre>
</div>

### MaxBotix Distance Sensors

#### The 5-meter sensors (MB7360, MB7369, MB7380, and MB7389) use a scale factor of (Vcc/5120) per 1-mm.
<pre>
Particle 12bit resolution (0-4095)
Sensor has a resolution of 0 - 5119mm
Each unit of the 0-4095 resolution is 1.25mm

Feather has 10bit resolution (0-1023)
Sensor has a resolution of 0 - 5119mm
Each unit of the 0-1023 resolution is 5mm
</pre>
#### The 10-meter sensors (MB7363, MB7366, MB7383, and MB7386) use a scale factor of (Vcc/10240) per 1-mm.
<pre>
Particle 12bit resolution (0-4095)
Sensor has a resolution of 0 - 10239mm
Each unit of the 0-4095 resolution is 2.5mm

Feather has 10bit resolution (0-1023)
Sensor has a resolution of 0 - 10239mm
Each unit of the 0-1023 resolution is 10mm
</pre>