# Particle Web Hooks
[← Back to Top](../README.md)<BR>
[← Particle Console](ParticleConsole.md)

---
### Example of Particle Webhook for Full Station Observations (FS)

<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>

{
 "name": "station-01 -> FS for chordsrt.com",
 "event": "FS",
 "deviceID": "Place Device ID Here",
 "url": "<http://3d.chordsrt.com/measurements/url_create>",
 "requestType": "GET",
 "noDefaults": true,
 "rejectUnauthorized": false,
 "query": {
     "key": "Place Chords Token ID Here",
     "instrument_id": "Place Chords Station ID Here",
     "at": "{{at}}",
     "hth": "{{hth}}",
     "bcs": "{{bcs}}",
     "bpc": "{{bpc}}",
     "cfr": "{{cfr}}",
     "css": "{{css}}",
     "bp1": "{{bp1}}",
     "bt1": "{{bt1}}",
     "bh1": "{{bh1}}",
     "hh1": "{{hh1}}",
     "ht1": "{{ht1}}",
     "st1": "{{st1}}",
     "sh1": "{{sh1}}",
     "st2": "{{st2}}",
     "sh2": "{{sh2}}",
     "hdt1": "{{hdt1}}",
     "hdh1": "{{hdh1}}",
     "hdt2": "{{hdt2}}",
     "hdh2": "{{hdh2}}",
     "sv1": "{{sv1}}",
     "si1": "{{si1}}",
     "su1": "{{su1}}",
     "gt1": "{{gt1}}",
     "gt2": "{{gt2}}",
     "vlx": "{{vlx}}",
     "blx": "{{blx}}",
     "mt1": "{{mt1}}",
     "rg": "{{rg}}",
     "rgt": "{{rg}}",
     "rgp": "{{rgp}}",
     "rg2": "{{rg2}}",
     "rgt2": "{{rgt2}}",
     "rgp2": "{{rgp2}}",
     "op1r": "{{op1r}}",
     "op2r": "{{op2r}}",
     "ws": "{{ws}}",
     "wd": "{{wd}}",
     "wg": "{{wg}}",
     "wgd": "{{wgd}}",
     "sg1": "{{sg1}}",
     "sg1v": "{{sg1v}}",
     "sg1p1": "{{sg1p1}}",
     "sg1t1": "{{sg1t1}}",
     "sg1h1": "{{sg1h1}}",
     "sg1p2": "{{sg1p2}}",
     "sg1t2": "{{sg1t2}}",
     "sg1h2": "{{sg1h2}}",
     "pm1s10": "{{pm1s10}}",
     "pm1s25": "{{pm1s25}}",
     "pm1s100": "{{pm1s100}}",
     "pm1e10": "{{pm1e10}}",
     "pm1e25": "{{pm1e25}}",
     "pm1e100": "{{pm1e100}}",
     "tlww": "{{tlww}}",
     "tlwt": "{{tlwt}}",       // Below are MUX connected Soil Moisture Possibilities
     "tsme25": "{{tsme25}}",   // tsme25-[1-8]
     "tsmeec": "{{tsmeec}}"    // tsmeec-[1-8]
     "tsmvwc": "{{tsmvwc}}",   // tsmvwc-[1-8]
     "tsmt": "{{tsmt}}",       // tsmt-[1-8]
     "tmsms1": "{{tmsms1}}",
     "tmsms2": "{{tmsms2}}",
     "tmsms3": "{{tmsms3}}",
     "tmsms4": "{{tmsms4}}",
     "tmsmt1": "{{tmsmt1}}",
     "tmsmt2": "{{tmsmt2}}"
  }
}
</pre>
</div>
Example of Particle Webhook for Feather LoRaRemoteMO deployments.
If you define the below with no "deviceID". The webhook can be used by multiple devices.
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
{
 "name": "LR for chordsrt.com",
 "event": "LR",
 "deviceID": "Place Device ID Here",
 "url": "<http://3d.chordsrt.com/measurements/url_create>",
 "requestType": "GET",
 "noDefaults": true,
 "rejectUnauthorized": false,
 "query": {
     "key": "Place Chords Token ID Here",
     "instrument_id": "{{id}}",
     "at": "{{at}}",
     "rg": "{{rg}}",
     "sg": "{{sg}}",
     "st1": "{{st1}}",
     "sm1": "{{sm1}}",
     "st2": "{{st2}}",
     "sm2": "{{sm2}}",
     "bp1": "{{bp1}}",
     "bt1": "{{bt1}}",
     "bh1": "{{bh1}}",
     "bp2": "{{bp2}}",
     "bt2": "{{bt2}}",
     "bh2": "{{bh2}}",
     "st1": "{{st1}}",
     "sh1": "{{sh1}}",
     "st2": "{{st2}}",
     "sh2": "{{sh2}}",
     "mt1": "{{mt1}}",
     "mt2": "{{mt2}}",
     "bv": "{{bv}}",
     "hth": "{{hth}}"
  }
}
</pre>
</div>