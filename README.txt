Line#		   Attribute
-----              --------------
 0                 0 - Color Image; 1 - Gray scale image
 1                 Eye coordinates
 2                 View direction vector
 3                 Sx - Viewing plane horizontal scale
 4                 Xmax,Ymax - Resultant Image Resolution
 5                 Per pixel sampleing in x & y directions; m,n
 6                 Distance from Eye coordinates to viewing plane
 7                 Camera up vector
 -                 Number of lights in scene
 8                 Light Position
 9                 Light source color specified in 0 to 1 range is scaled to MaxRGB value in program
 10                Specular light color
 11                Ambient light color
 12                Ambient light intensity
 13                Phong exponent
 14                Diffuse color factor used in cool color computation
 15                Diffuse color factor used in warm color computation


              // Resultant color computation
              /*Below one includes Lambert shading*/
              //curr_result = cos_theta*(Cl*Ch_tmin);

              /*Below one includes Lambert and Phong Illumination
              curr_result = ((1-cos_phi)*cos_theta*(Cl*Ch_tmin) + cos_phi*(data.specL*Cl));*/

              /*Below includes Lambert and Gooch & Gooch Shading.
              PixColor CF = (1-cos_theta)*data.Cc + cos_theta*data.Cw;
              curr_result =((1-cos_phi)*cos_theta*(CF*Cl)+cos_phi*(data.specL*Cl));*/
-3.0 -4.0 -5.0

Area Light
----------
1
-5.0 -5.0 -5.0 1.0 0.0 0.0 0.0 1.0 0.0 4 4 1.0 1.0 1.0

1
-2 -8 -6 1 1 1

0.855 0.541 0.404 - copper
0.753 0.753 0.753 - silver
0.765 0.129 0.282 - maroon

Area Light LGS shader
---------------------
                    /*Below includes Lambert, Phong and Gooch & Gooch*/
                    PixColor K_cool = alpha*Ch_tmin + Cc;
                    PixColor K_warm = beta*Ch_tmin + Cw;
                    ALi_SAMPLE = (((1-cos_phi)*((1-cos_theta)*K_cool + cos_theta*K_warm)+cos_phi*specL)*Cl);


Point Light LGS Shader
----------------------

/*Below includes Lambert, Phong and Gooch & Gooch
            PixColor K_cool = alpha*Ch_tmin + Cc;
            PixColor K_warm = beta*Ch_tmin + Cw;
            curr_sample = (((1-cos_phi)*((1-cos_theta)*K_cool + cos_theta*K_warm)+cos_phi*specL)*Cl);*/


/*Below one includes Lambert and Phong Illumination*/
            curr_sample = ((1-cos_phi)*cos_theta*(Cl*Ch_tmin) + cos_phi*(specL*Cl));

/*Below includes Lambert, Phong and Gooch & Gooch*/
                    PixColor K_cool = alpha*Ch_tmin + Cc;
                    PixColor K_warm = beta*Ch_tmin + Cw;
                    curr_sample = (((1-cos_phi)*((1-cos_theta)*K_cool + cos_theta*K_warm)+cos_phi*specL)*Cl);


Object File format <Finish this later>
settings for dodecohandle & cubehandle

-2.0 -6.0 -6.0
0.0 1.0 1.0
8
640 480
1 1
3



LIGHTS
------
3
0 -4 -6 1 1 1
-2 0 -8 1 1 1
2 0 -8 1 1 1
1
0.0 -2.0 -8.0 1.0 0.0 0.0 0.0 1.0 0.0 6 6 1.0 1.0 1.0

New Code TEST CASES
-----------------------------------------------

TEST CASE 1

Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 2.0 0.0 1.0 1.0 1.0 0 0 0 0 -1 0 0
1.0 1.0 1.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/uparrow.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 3.0 2.0 2.0 2.0 1 1 1 0 0 0 -1
1.0 0.0 0.0 0 2 1.52
/home/prady/Qt/C++/ImageSynthesis/TextureImages/plasma.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 -3.5 3.0 1.5 1.5 1.5 1 1 1 0 0 0 -1
1.0 1.0 0.0 0 1 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/plasma.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 -6.0 3.0 1.0 1.0 1.0 1 1 1 0 0 0 -1
0.0 0.0 1.0 0 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/plasma.jpg
0 0 0 0


TEST CASE 2

Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 -6.5 6.0 6.0 6.0 0 0 0 0 0 1 0
1.0 1.0 1.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/linuxtux.png
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 2.0 0.0 1.0 1.0 1.0 0 0 0 0 -1 0 0
1.0 1.0 1.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/uparrow.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 7.0 4.0 4.0 4.0 0 0 0 0 0 -1 0
0.9 0.9 0.9 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/Q.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 15.0 0.0 0.0 1.0 1.0 1.0 0 0 0 -1 0 0 0
0.3 0.5 0.3 0 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/platemetal.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 -15.0 0.0 0.0 1.0 1.0 1.0 0 0 0 1 0 0 0
1.0 0.78 0.69 0 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/platemetal.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 -15.0 0.0 2.0 2.0 2.0 0 0 0 0 1 0 0
1.0 1.0 1.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/rough.jpg
0 0 0 0
Q -2.0 0.0 0.0 0.0 -2.0 0.0 -5.0 -2.0 -3.0 0.2 1.0 0.2 1 -1 1 0 0 0 0
0.8 0.8 0.0 0 1 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/brownpaper.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 5.5 -0.5 -3.0 0.25 0.75 0.2 1 -1 1 0 0 0 -1
0.0 0.8 0.0 0 2 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/crate.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 -0.5 -2.0 -2.5 0.25 0.75 0.5 1 -1 1 0 0 0 1
0.0 0.0 0.6 0 1 0.1
/home/prady/Qt/C++/ImageSynthesis/TextureImages/rough.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 2.5 -0.5 -3.0 2 1.75 1.75 1 1 1 0 0 0 -1
0.8 0.2 0.0 0 1 0.1
/home/prady/Qt/C++/ImageSynthesis/TextureImages/earth.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 1.0 -6.0 0.25 0.35 0.35 0 1 1 0 0 0 -1
1.0 0.0 0.0 0 1 0.1
/home/prady/Qt/C++/ImageSynthesis/TextureImages/Grid.jpg
0 0 0 0

TEST CASE 3

Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 2.0 0.0 1.0 1.0 1.0 0 0 0 0 -1 0 0
0.9 0.6 0.3 0 0 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/uparrow.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 3.0 1.0 1.0 1.0 0 0 0 0 0 -1 0
1.0 1.0 1.0 1 0 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/Q.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 3.0 -2.0 2.0 2.0 2.0 2.0 1 1 1 0 0 0 -1
1.0 0.0 0.0 0 2 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/rough.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 -3.5 -2.0 -0.5 0.25 0.75 0.5 1 -1 1 0 0 0 1
0.0 0.0 0.6 0 1 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/rough.jpg
0 0 0 0

TEST CASE 4

Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 -1.0 1.0 1.0 1.0 0 0 0 0 -1 0 0
1.0 0.5 0.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/ground.jpg
0 0 0 0
Q 1.0 0.0 0.0 0.0 1.0 0.0 0.0 -2.0 -4.0 2.0 2.0 2.0 1 1 1 0 0 0 -1
0.0 0.5 1.0 1 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/Grid.jpg
0 0 0 0


TEST CASE 5

S 0.0 -2.0 -4.0 2.0 -1.0 0.0 0.0 0.0 1.0 0.0
1.0 0.0 0.0 0 2 1.33
/home/prady/Qt/C++/ImageSynthesis/TextureImages/linuxtux.png
0 0 0 0
P 0.0 0.0 -1.0 0.0 0.0 -1.0 -1.0 0.0 0.0 0.0 1.0 0.0
1.0 1.0 1.0 0 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/uparrow.jpg
0 0 0 0

TEST CASE 6

K 1
0.0 1.0 0.0 0 0 0
/home/prady/Qt/C++/ImageSynthesis/TextureImages/linuxtux.png
0 0 0 0
S 0.0 0.0 -3.0 2.0 1.0 0.0 0.0 0.0 1.0 0.0
S 0.0 0.0 -4.0 2.0 1.0 0.0 0.0 0.0 1.0 0.0
P 0.0 -1.0 0.0 0.0 0.0 1.0 1.0 0.0 0.0 0.0 1.0 0.0
1.0 1.0 1.0 0 1 1.52
/home/prady/Qt/C++/ImageSynthesis/TextureImages/uparrow.jpg
0 0 0 0

TEST CASE 7

S 0.0 -1.0 -3.0 3.0 -1.0 0.0 0.0 0.0 1.0 0.0
1.0 0.0 0.0 1 0 1.33
./TextureImages/linuxtux.png
0 0 0 0
M
./Triangular/square.obj
1.0 1.0 1.0 0 0 0
./TextureImages/Q.jpg
0 0 0 0

TEST CASE 8

S 0.0 -2.0 -2.0 5.0 1.0 0.0 0.0 0.0 1.0 0.0 1.0 1.0 1.0
0.761 0.698 0.502 1 0 0
./TextureImages/checkerboard.jpg
0 0 0 0
P 0.0 -1.0 0.0 0.0 0.0 -1.0 1.0 0.0 0.0 0.0 1.0 0.0 1.0 1.0 1.0
0.882 0.663 0.373 1 0 0
./TextureImages/rough.jpg
0 0 0 0


TEST CASE 9

M
./Triangular/nCube.obj
1.0 1.0 1.0 3 0 0
./TextureImages/crate.jpg
0 0 0 0
P 0.0 -1.0 0.0 0.0 3.0 -1.0 1.0 0.0 0.0 0.0 1.0 0.0 8.0 8.0 8.0
0.882 0.663 0.373 1 0 0
./TextureImages/ground.jpg
0 0 0 0

