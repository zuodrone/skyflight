<div><span style="white-space:pre"></span><img src="https://raw.githubusercontent.com/zuodrone/skyflight/master/doc/skyflight.png"></div>

<div># skyflight &nbsp; &nbsp;by ZUODRONE</div>
<div>---------------------------------------</div>
<div>基于stm32f10x的飞控代码。</div>
<div>---------------------------------------</div>
<div>log:</div> 
<div>2016.09.11 &nbsp;v1.0</div>
<div><span style="white-space:pre">	</span>
姿態解析實現。
</div>
<div><br></div>
<div>2016.09.09 &nbsp;v1.0</div>
<div><span style="white-space:pre">	</span>
实现了4通道摇控PPM解码，MPU6050参数读取，卡尔曼滤波，PWM（电机驱动）。
</div>
<div><span style="white-space:pre">	</span>
开发计划：mavlink 融合。
</div>
<div><br></div><div><br></div><div><br></div><div><br></div><div><br></div><div><br></div><div><br></div><div><br></div><div><br></div><div><br></div>
<div>----------------------------------------</div>
<div>編譯過程（以UBUNTU爲例）</div>
<div>1、安裝gcc-arm-none-eabi-4_8-2013q4編譯環境。</div>
<div><span style="white-space:pre">	</span>地址：http://pan.baidu.com/s/1o8ETbZW</div>
<div><br></div>
<div>2、clone工程</div>
<div><span style="white-space:pre">	</span>git clone https://github.com/zuodrone/skyflight.git</div>
<div><br></div>
<div>3、編譯</div> 
<div><span style="white-space:pre">	</span>cd skyflight</div>
<div><span style="white-space:pre">	</span>./build.sh</div>
<div><span style="white-space:pre">	</span>執行以上兩步，等待編譯完成。</div>
<div>4、下載</div>
<div><span style="white-space:pre">	</span>使用stm32f10x開發板對應的下載器下載即可。</div>
<div><br></div>
<div>
<span style="white-space:pre"></span>左航創新</div>
