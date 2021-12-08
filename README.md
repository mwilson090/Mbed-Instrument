<h1>mbed-instrument</h1>

<p>The mbed instrument plays music based on motion. It connects to wifi, displaying the song play on a webpage. webpage that outputs the song played.</p>

<h2>How to Play</h2>
<p>-manuever the board </p>
<p>-x-axis: dictates notes</p>
<p>-y-axis: dictates volume</p>
<p>-button: press to play / release to rest</p>

<h2>Components</h2>
<ul>
  <li>Mbed</li>
  <li>Push Button</li>
  <li>RGB Led</li>
  <li>MMA8452 Accelerometer</li>
  <li>Class D High-Efficiency Audio Amp</li>
  <li>Speaker</li>
  <li>ESP8266 Huzzah</li>
  <li>External 5V Supply</li>
</ul>

<h2>Wiring</h2>

<table>
  <tr>
    <th>Mbed</th>
    <th>MMA8452 Accelerometer</th>
  </tr>
  <tr>
    <td>v+</td>
    <td>3.3V</td>
  </tr>
  <tr>
    <td>p9</td>
    <td>sda</td>
  </tr>
  <tr>
    <td>p10</td>
    <td>scl</td>
  </tr>
  <tr>
    <td>gnd</td>
    <td>gnd</td>
  </tr>
</table>

<table>
  <tr>
    <th>Mbed</th>
    <th>Class D High-Efficiency Audio Amp</th>
    <th>Speaker</th>
  </tr>
  <tr>
    <td>gnd</td>
    <td>pwr - (gnd), in -</td>
    <td></td>
  </tr>
  <tr>
    <td>Vout (3.3V) or 5V</td>
    <td>pwr +</td>
    <td></td>
  </tr>
    <tr>
    <td>p26 (any PWM)</td>
    <td>pwr +</td>
    <td></td>
  </tr>
    <tr>
    <td>in +</td>
    <td>pwr +</td>
    <td></td>
  </tr>
    <tr>
    <td></td>
    <td>out +</td>
    <td>+</td>
  </tr>
    </tr>
    <tr>
    <td></td>
    <td>	out -</td>
    <td>-</td>
  </tr>
</table>

<table>
  <tr>
    <th>Huzzah</th>
    <th>Mbed</th>
    <th>External 5V Supply</th>
  </tr>
  <tr>
    <td>gnd</td>
    <td>gnd</td>
    <td>gnd</td>
  </tr>
  <tr>
    <td>tx</td>
    <td>rx- p27</td>
    <td></td>
  </tr>
  <tr>
    <td>rx</td>
    <td>tx- p28</td>
    <td></td>
  </tr>
    <tr>
    <td>v+</td>
    <td></td>
    <td>5V DC</td>
  </tr>
    <tr>
    <td>rest</td>
    <td>p26 (Optional)</td>
    <td></td>
  </tr>
</table>

<h2>Set Up</h2>
<ol>
  <li>Complete wiring connections.</li>
  <li>Upload code to the mbed compiler. Change "ssid" and "password" to your wifi name and password and compile code to the mbed.</li>
  <li>Make sure your computer is connected to the same wifi network in the code. Open TeraTerm and connect to your mbed. Press mbed reset button.</li>
  <li>Go to the IP address outputed on TeraTerm.</li>
  <li>Select start on the webpage and click submit. Now you can play the instrument!</li>
  <li>Select stop on the webpage and click submit to see the song you just played.</li>
</ol>

<h2>Video Demo</h2>
<p>Add!</p>
