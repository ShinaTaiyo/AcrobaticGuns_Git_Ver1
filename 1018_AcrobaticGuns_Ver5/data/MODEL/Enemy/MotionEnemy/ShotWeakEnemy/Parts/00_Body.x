xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 132;
 -6.40291;12.97245;-5.76262;,
 6.40291;12.97245;-5.76262;,
 6.40291;0.16663;-5.76262;,
 -6.40291;0.16663;-5.76262;,
 6.40291;12.97245;-5.76262;,
 6.40291;12.97245;5.76262;,
 6.40291;0.16663;5.76262;,
 6.40291;0.16663;-5.76262;,
 6.40291;12.97245;5.76262;,
 -6.40291;12.97245;5.76262;,
 -6.40291;0.16663;5.76262;,
 6.40291;0.16663;5.76262;,
 -6.40291;12.97245;5.76262;,
 -6.40291;12.97245;-5.76262;,
 -6.40291;0.16663;-5.76262;,
 -6.40291;0.16663;5.76262;,
 6.40291;12.97245;-5.76262;,
 -6.40291;12.97245;-5.76262;,
 -6.40291;0.16663;-5.76262;,
 6.40291;0.16663;-5.76262;,
 -8.64912;14.02971;5.59121;,
 -8.11134;14.40627;5.59121;,
 3.10694;-1.61511;5.59121;,
 2.56916;-1.99166;5.59121;,
 -7.57356;14.78283;5.59121;,
 3.64473;-1.23854;5.59121;,
 -7.03577;15.15939;5.59121;,
 4.18251;-0.86198;5.59121;,
 4.49743;-3.39490;5.59121;,
 4.15325;-3.63590;5.59121;,
 4.84162;-3.15390;5.59121;,
 5.18580;-2.91291;5.59121;,
 5.26537;-4.14826;5.59121;,
 5.24387;-4.16332;5.59121;,
 5.28688;-4.13320;5.59121;,
 5.30840;-4.11813;5.59121;,
 -7.03577;15.15939;5.59121;,
 -7.03577;15.15939;6.02195;,
 4.18251;-0.86198;6.02195;,
 4.18251;-0.86198;5.59121;,
 -7.03577;15.15939;6.45269;,
 4.18251;-0.86198;6.45269;,
 -7.03577;15.15939;6.88343;,
 4.18251;-0.86198;6.88343;,
 5.18580;-2.91291;6.02195;,
 5.18580;-2.91291;5.59121;,
 5.18580;-2.91291;6.45269;,
 5.18580;-2.91291;6.88343;,
 5.30840;-4.11813;6.02195;,
 5.30840;-4.11813;5.59121;,
 5.30840;-4.11813;6.45269;,
 5.30840;-4.11813;6.88343;,
 -7.03577;15.15939;6.88343;,
 -7.57356;14.78283;6.88343;,
 3.64473;-1.23854;6.88343;,
 4.18251;-0.86198;6.88343;,
 -8.11134;14.40627;6.88343;,
 3.10694;-1.61511;6.88343;,
 -8.64912;14.02971;6.88343;,
 2.56916;-1.99166;6.88343;,
 4.84162;-3.15390;6.88343;,
 5.18580;-2.91291;6.88343;,
 4.49743;-3.39490;6.88343;,
 4.15325;-3.63590;6.88343;,
 5.28688;-4.13320;6.88343;,
 5.30840;-4.11813;6.88343;,
 5.26537;-4.14826;6.88343;,
 5.24387;-4.16332;6.88343;,
 -8.64912;14.02971;6.88343;,
 -8.64912;14.02971;6.45269;,
 2.56916;-1.99166;6.45269;,
 2.56916;-1.99166;6.88343;,
 -8.64912;14.02971;6.02195;,
 2.56916;-1.99166;6.02195;,
 -8.64912;14.02971;5.59121;,
 2.56916;-1.99166;5.59121;,
 4.15325;-3.63590;6.45269;,
 4.15325;-3.63590;6.88343;,
 4.15325;-3.63590;6.02195;,
 4.15325;-3.63590;5.59121;,
 5.24387;-4.16332;6.45269;,
 5.24387;-4.16332;6.88343;,
 5.24387;-4.16332;6.02195;,
 5.24387;-4.16332;5.59121;,
 -8.11134;14.40627;6.88343;,
 -8.47648;14.15059;6.74515;,
 -8.64912;14.02971;6.45269;,
 -7.57356;14.78283;6.88343;,
 -7.20842;15.03850;6.74515;,
 -7.03577;15.15939;6.45269;,
 -8.47648;14.15059;5.72949;,
 -8.64912;14.02971;6.02195;,
 -7.03577;15.15939;6.02195;,
 -7.20842;15.03850;5.72949;,
 -8.11134;14.40627;5.59121;,
 -8.64912;14.02971;5.59121;,
 -7.57356;14.78283;5.59121;,
 -7.03577;15.15939;5.59121;,
 5.24387;-4.16332;6.45269;,
 5.26537;-4.14826;6.45269;,
 5.26537;-4.14826;6.88343;,
 5.28688;-4.13320;6.45269;,
 5.28688;-4.13320;6.88343;,
 5.30840;-4.11813;6.45269;,
 5.24387;-4.16332;6.02195;,
 5.26537;-4.14826;6.02195;,
 5.28688;-4.13320;6.02195;,
 5.30840;-4.11813;6.02195;,
 5.24387;-4.16332;5.59121;,
 5.26537;-4.14826;5.59121;,
 5.28688;-4.13320;5.59121;,
 5.30840;-4.11813;5.59121;,
 -7.29000;3.86815;-7.29000;,
 7.29000;3.86815;-7.29000;,
 7.29000;2.04565;-7.29000;,
 -7.29000;2.04565;-7.29000;,
 7.29000;3.86815;-7.29000;,
 7.29000;3.86815;7.29000;,
 7.29000;2.04565;7.29000;,
 7.29000;2.04565;-7.29000;,
 7.29000;3.86815;7.29000;,
 -7.29000;3.86815;7.29000;,
 -7.29000;2.04565;7.29000;,
 7.29000;2.04565;7.29000;,
 -7.29000;3.86815;7.29000;,
 -7.29000;3.86815;-7.29000;,
 -7.29000;2.04565;-7.29000;,
 -7.29000;2.04565;7.29000;,
 7.29000;3.86815;-7.29000;,
 -7.29000;3.86815;-7.29000;,
 -7.29000;2.04565;-7.29000;,
 7.29000;2.04565;-7.29000;;
 
 119;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;21,24,25,22;,
 4;24,26,27,25;,
 4;23,22,28,29;,
 4;22,25,30,28;,
 4;25,27,31,30;,
 4;29,28,32,33;,
 4;28,30,34,32;,
 4;30,31,35,34;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;40,42,43,41;,
 4;39,38,44,45;,
 4;38,41,46,44;,
 4;41,43,47,46;,
 4;45,44,48,49;,
 4;44,46,50,48;,
 4;46,47,51,50;,
 4;52,53,54,55;,
 4;53,56,57,54;,
 4;56,58,59,57;,
 4;55,54,60,61;,
 4;54,57,62,60;,
 4;57,59,63,62;,
 4;61,60,64,65;,
 4;60,62,66,64;,
 4;62,63,67,66;,
 4;68,69,70,71;,
 4;69,72,73,70;,
 4;72,74,75,73;,
 4;71,70,76,77;,
 4;70,73,78,76;,
 4;73,75,79,78;,
 4;77,76,80,81;,
 4;76,78,82,80;,
 4;78,79,83,82;,
 4;68,84,85,86;,
 4;84,87,88,85;,
 4;87,42,89,88;,
 4;86,85,90,91;,
 4;88,89,92,93;,
 4;91,90,94,95;,
 4;90,93,96,94;,
 4;93,92,97,96;,
 4;98,99,100,81;,
 4;99,101,102,100;,
 4;101,103,51,102;,
 4;104,105,99,98;,
 4;105,106,101,99;,
 4;106,107,103,101;,
 4;108,109,105,104;,
 4;109,110,106,105;,
 4;110,111,107,106;,
 4;23,22,21,20;,
 4;22,25,24,21;,
 4;25,27,26,24;,
 4;29,28,22,23;,
 4;28,30,25,22;,
 4;30,31,27,25;,
 4;33,32,28,29;,
 4;32,34,30,28;,
 4;34,35,31,30;,
 4;39,38,37,36;,
 4;38,41,40,37;,
 4;41,43,42,40;,
 4;45,44,38,39;,
 4;44,46,41,38;,
 4;46,47,43,41;,
 4;49,48,44,45;,
 4;48,50,46,44;,
 4;50,51,47,46;,
 4;55,54,53,52;,
 4;54,57,56,53;,
 4;57,59,58,56;,
 4;61,60,54,55;,
 4;60,62,57,54;,
 4;62,63,59,57;,
 4;65,64,60,61;,
 4;64,66,62,60;,
 4;66,67,63,62;,
 4;71,70,69,68;,
 4;70,73,72,69;,
 4;73,75,74,72;,
 4;77,76,70,71;,
 4;76,78,73,70;,
 4;78,79,75,73;,
 4;81,80,76,77;,
 4;80,82,78,76;,
 4;82,83,79,78;,
 4;86,85,84,68;,
 4;85,88,87,84;,
 4;88,89,42,87;,
 4;91,90,85,86;,
 4;90,93,88,85;,
 4;93,92,89,88;,
 4;95,94,90,91;,
 4;94,96,93,90;,
 4;96,97,92,93;,
 4;81,100,99,98;,
 4;100,102,101,99;,
 4;102,51,103,101;,
 4;98,99,105,104;,
 4;99,101,106,105;,
 4;101,103,107,106;,
 4;104,105,109,108;,
 4;105,106,110,109;,
 4;106,107,111,110;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;124,125,126,127;,
 4;124,117,128,129;,
 4;130,131,118,127;;
 
 MeshMaterialList {
  16;
  119;
  0,
  0,
  0,
  0,
  0,
  0,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  13,
  14,
  14,
  14,
  14,
  14,
  14;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.674510;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.480000;0.721600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.454902;0.527059;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.721569;0.401569;0.721569;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.599216;0.134902;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.520784;0.094118;0.094118;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.480000;0.721569;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  68;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.819152;0.573576;0.000000;,
  0.861331;0.508044;0.000000;,
  0.961560;0.274594;0.000000;,
  0.994866;0.101200;0.000000;,
  0.819152;0.573576;0.000000;,
  0.961560;0.274594;0.000000;,
  -0.819152;-0.573576;0.000000;,
  -0.771998;-0.635625;0.000000;,
  -0.586907;-0.809654;0.000000;,
  -0.435362;-0.900256;0.000000;,
  -0.573575;0.819153;-0.000005;,
  -0.573581;0.819149;-0.000007;,
  -0.573575;0.819153;0.000005;,
  -0.573581;0.819149;0.000007;,
  0.573588;-0.819144;-0.000000;,
  0.573579;-0.819150;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.819152;-0.573576;0.000000;,
  -0.435362;-0.900256;0.000000;,
  -0.573574;0.819153;-0.000001;,
  -0.573576;0.819152;-0.000007;,
  -0.573574;0.819154;-0.000001;,
  -0.573579;0.819150;-0.000010;,
  -0.573580;0.819149;-0.000008;,
  -0.573583;0.819148;-0.000004;,
  -0.573574;0.819154;0.000001;,
  -0.573583;0.819148;0.000004;,
  -0.573576;0.819152;0.000007;,
  -0.573575;0.819153;0.000001;,
  -0.573579;0.819150;0.000010;,
  -0.573580;0.819149;0.000008;,
  0.573572;-0.819155;-0.000000;,
  0.573554;-0.819168;-0.000000;,
  0.573554;-0.819168;-0.000000;,
  -0.861331;-0.508044;0.000000;,
  -0.961560;-0.274594;0.000000;,
  -0.961560;-0.274594;0.000000;,
  -0.994866;-0.101200;0.000000;,
  0.771998;0.635625;0.000000;,
  0.586907;0.809654;0.000000;,
  0.435362;0.900256;0.000000;,
  0.435362;0.900256;0.000000;,
  0.573574;-0.819154;0.000001;,
  0.573575;-0.819153;0.000003;,
  0.573576;-0.819152;0.000007;,
  0.573574;-0.819153;0.000001;,
  0.573580;-0.819150;0.000005;,
  0.573579;-0.819150;0.000010;,
  0.573583;-0.819148;0.000004;,
  0.573580;-0.819149;0.000008;,
  0.573574;-0.819154;-0.000001;,
  0.573575;-0.819153;-0.000003;,
  0.573580;-0.819150;-0.000005;,
  0.573583;-0.819148;-0.000004;,
  0.573575;-0.819153;-0.000001;,
  0.573576;-0.819152;-0.000007;,
  0.573579;-0.819150;-0.000010;,
  0.573580;-0.819149;-0.000008;,
  -0.573572;0.819155;0.000000;,
  -0.573588;0.819144;0.000000;,
  -0.573579;0.819150;0.000000;,
  -0.573554;0.819168;0.000000;,
  -0.573554;0.819168;0.000000;;
  119;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;7,3,4,4;,
  4;3,3,4,4;,
  4;3,7,4,4;,
  4;4,4,5,8;,
  4;4,4,5,5;,
  4;4,4,8,5;,
  4;8,5,6,6;,
  4;5,5,6,6;,
  4;5,8,6,6;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;22,9,10,10;,
  4;9,9,10,10;,
  4;9,22,10,10;,
  4;10,10,11,11;,
  4;10,10,11,11;,
  4;10,10,11,11;,
  4;11,11,12,23;,
  4;11,11,12,12;,
  4;11,11,23,12;,
  4;24,25,13,26;,
  4;25,27,14,13;,
  4;27,28,29,14;,
  4;26,13,15,30;,
  4;14,29,31,16;,
  4;30,15,32,33;,
  4;15,16,34,32;,
  4;16,31,35,34;,
  4;36,17,17,36;,
  4;17,18,18,17;,
  4;18,37,38,18;,
  4;36,17,17,36;,
  4;17,18,18,17;,
  4;18,37,37,18;,
  4;36,17,17,36;,
  4;17,18,18,17;,
  4;18,38,37,18;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;39,39,9,22;,
  4;39,39,9,9;,
  4;39,39,22,9;,
  4;40,41,39,39;,
  4;41,41,39,39;,
  4;41,40,39,39;,
  4;42,42,41,40;,
  4;42,42,41,41;,
  4;42,42,40,41;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;43,43,3,7;,
  4;43,43,3,3;,
  4;43,43,7,3;,
  4;44,44,43,43;,
  4;44,44,43,43;,
  4;44,44,43,43;,
  4;45,46,44,44;,
  4;46,46,44,44;,
  4;46,45,44,44;,
  4;47,48,49,50;,
  4;48,51,52,49;,
  4;51,53,54,52;,
  4;55,56,48,47;,
  4;56,57,51,48;,
  4;57,58,53,51;,
  4;59,60,56,55;,
  4;60,61,57,56;,
  4;61,62,58,57;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,66,67,65;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,67,67,65;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,67,66,65;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;;
 }
 MeshTextureCoords {
  132;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.333333;0.000000;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.666667;0.000000;,
  0.666667;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.666667;0.666667;,
  1.000000;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333333;0.000000;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.666667;0.000000;,
  0.666667;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.666667;0.666667;,
  1.000000;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333333;0.000000;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.666667;0.000000;,
  0.666667;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.666667;0.666667;,
  1.000000;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333333;0.000000;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.666667;0.000000;,
  0.666667;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.666667;0.666667;,
  1.000000;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;1.000000;,
  1.000000;1.000000;,
  0.333333;0.000000;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.666667;0.000000;,
  0.666667;0.333333;,
  1.000000;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  1.000000;0.666667;,
  0.666667;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;1.000000;,
  1.000000;1.000000;,
  0.000000;0.666667;,
  0.333333;0.666667;,
  0.333333;1.000000;,
  0.666667;0.666667;,
  0.666667;1.000000;,
  1.000000;0.666667;,
  0.000000;0.333333;,
  0.333333;0.333333;,
  0.666667;0.333333;,
  1.000000;0.333333;,
  0.000000;0.000000;,
  0.333333;0.000000;,
  0.666667;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}