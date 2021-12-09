<h1>mbed-instrument</h1>

<p>The mbed instrument plays music based on motion. It connects to wifi, a webpage enables it to start/stop playing and can set an initial volume. </p>

<img src= "https://github.gatech.edu/storage/user/20775/files/572f57d0-301e-480f-99d9-6740fc22419a" />

<h2>Components</h2>
<ul>
  <li>Mbed</li>
  <li>Push Button (Uses Internal Pullup)</li>
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
    <th>RGB Led</th>
  </tr>
  <tr>
    <td>p25</td>
    <td>red</td>
  </tr>
  <tr>
    <td>p24</td>
    <td>green</td>
  </tr>
  <tr>
    <td>p23</td>
    <td>blue</td>
  </tr>
  <tr>
    <td>gnd</td>
    <td>gnd</td>
  </tr>
</table>

<img src="https://github.gatech.edu/storage/user/20775/files/f814c105-3215-4edf-9cc5-7f532b86413c" />

<table>
  <tr>
    <th>Mbed</th>
    <th>Push Button</th>
  </tr>
  <tr>
    <td>p21</td>
    <td>C</td>
  </tr>
  <tr>
    <td>gnd</td>
    <td>A</td>
  </tr>
</table>

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
    <td>p27</td>
    <td>sda</td>
  </tr>
  <tr>
    <td>p28</td>
    <td>scl</td>
  </tr>
  <tr>
    <td>gnd</td>
    <td>gnd</td>
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
    <td>rx- p9</td>
    <td></td>
  </tr>
  <tr>
    <td>rx</td>
    <td>tx- p10</td>
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

<p>The external 5V DC barrel jack is connected to ground from the mbed and outputs 5V power. </p>
<img src="https://github.gatech.edu/storage/user/20775/files/cb19a71a-bd11-422f-bdec-170441f5deaf" />

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

<h2>Set Up</h2>
<ol>
  <li>Complete wiring connections.</li>
  <li>Upload code to the mbed compiler. Change "ssid" and "pwd" to your wifi name and password and compile code to the mbed.</li>
  <img src="https://github.gatech.edu/storage/user/20775/files/e191742f-7de6-4452-bab9-6ea9a929b0f0" />
  <li>Make sure your computer is connected to the same wifi network in the code. Open TeraTerm and connect to your mbed. Press mbed reset button.</li>
  <img src="https://github.gatech.edu/storage/user/20984/files/baa75624-a21d-4d0c-b9bd-4eae42bca201">
  <img src="https://github.gatech.edu/storage/user/20984/files/04ef696f-6e20-4f89-9d34-3680965225ac">
  <img src="https://github.gatech.edu/storage/user/20984/files/5cab3885-3dd8-4c32-91a3-49c6715b0c05">
  <img src="https://github.gatech.edu/storage/user/20984/files/d71921ff-f4f9-4d0f-8014-a848250821c2">
  <li>Once your terminal prints "Ready" go to the IP address outputed. </li>
  <img src="https://github.gatech.edu/storage/user/20775/files/e579036b-d6d4-40c4-adfb-c7b530fd235b" />
  <li>Select start on the webpage and click refresh. Now you can play the instrument!</li>
  <li>Select stop on the webpage and click refresh to stop playing.</li>
</ol>


<h2>How to Play</h2>
<p>-manuever the board </p>
<p>-x-axis: dictates notes</p>
<p>-y-axis: dictates volume</p>
<p>-button: press to play / release to rest</p>

<h2>Video Demo</h2>
https://youtu.be/kpXA_2L8fKo


