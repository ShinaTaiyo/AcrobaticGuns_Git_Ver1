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
 141;
 0.00000;14.06039;5.93961;,
 2.70598;11.35441;5.17840;,
 0.00000;10.23357;5.17840;,
 0.00000;14.06039;5.93961;,
 3.82683;14.06039;5.17840;,
 0.00000;14.06039;5.93961;,
 2.70598;16.76637;5.17840;,
 0.00000;14.06039;5.93961;,
 0.00000;17.88723;5.17840;,
 0.00000;14.06039;5.93961;,
 -2.70598;16.76637;5.17840;,
 0.00000;14.06039;5.93961;,
 -3.82683;14.06039;5.17840;,
 0.00000;14.06039;5.93961;,
 -2.70598;11.35441;5.17840;,
 0.00000;14.06039;5.93961;,
 0.00000;10.23357;5.17840;,
 5.00000;9.06039;3.01068;,
 0.00000;6.98933;3.01068;,
 7.07107;14.06039;3.01068;,
 5.00000;19.06039;3.01067;,
 0.00000;21.13146;3.01067;,
 -5.00000;19.06039;3.01067;,
 -7.07107;14.06039;3.01068;,
 -5.00000;9.06039;3.01068;,
 0.00000;6.98933;3.01068;,
 6.53281;7.52758;-0.23356;,
 0.00000;4.82160;-0.23356;,
 9.23879;14.06039;-0.23356;,
 6.53281;20.59320;-0.23357;,
 0.00000;23.29919;-0.23357;,
 -6.53282;20.59320;-0.23357;,
 -9.23879;14.06039;-0.23356;,
 -6.53282;7.52758;-0.23356;,
 0.00000;4.82160;-0.23356;,
 7.07107;6.98933;-4.06039;,
 0.00000;4.06039;-4.06039;,
 10.00000;14.06039;-4.06039;,
 7.07107;21.13146;-4.06039;,
 0.00000;24.06039;-4.06039;,
 -7.07107;21.13146;-4.06039;,
 -10.00000;14.06039;-4.06039;,
 -7.07107;6.98933;-4.06039;,
 0.00000;4.06039;-4.06039;,
 6.53281;7.52758;-7.88723;,
 0.00000;4.82160;-7.88723;,
 9.23879;14.06039;-7.88723;,
 6.53281;20.59320;-7.88723;,
 0.00000;23.29919;-7.88724;,
 -6.53282;20.59320;-7.88723;,
 -9.23879;14.06039;-7.88723;,
 -6.53282;7.52758;-7.88723;,
 0.00000;4.82160;-7.88723;,
 5.00000;9.06039;-11.13146;,
 0.00000;6.98932;-11.13146;,
 7.07107;14.06039;-11.13146;,
 5.00000;19.06039;-11.13146;,
 0.00000;21.13146;-11.13146;,
 -5.00000;19.06039;-11.13146;,
 -7.07107;14.06039;-11.13146;,
 -5.00000;9.06039;-11.13146;,
 0.00000;6.98932;-11.13146;,
 2.70598;11.35441;-13.29918;,
 0.00000;10.23356;-13.29918;,
 3.82683;14.06039;-13.29918;,
 2.70598;16.76637;-13.29918;,
 0.00000;17.88722;-13.29918;,
 -2.70598;16.76637;-13.29918;,
 -3.82683;14.06039;-13.29918;,
 -2.70598;11.35441;-13.29918;,
 0.00000;10.23356;-13.29918;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;14.06039;-14.06039;,
 0.00000;0.75039;14.06039;,
 0.00000;2.34760;12.22695;,
 -8.28220;5.77820;12.22695;,
 -9.41159;4.64881;14.06039;,
 -11.71280;14.06040;12.22695;,
 -13.31000;14.06040;14.06039;,
 -8.28220;22.34260;12.22695;,
 -9.41159;23.47199;14.06039;,
 0.00000;25.77320;12.22694;,
 0.00000;27.37040;14.06039;,
 8.28220;22.34260;12.22695;,
 9.41159;23.47199;14.06039;,
 11.71280;14.06040;12.22695;,
 13.31000;14.06040;14.06039;,
 8.28220;5.77820;12.22695;,
 9.41159;4.64881;14.06039;,
 0.00000;2.34760;12.22695;,
 0.00000;0.75039;14.06039;,
 0.00000;5.27580;9.70597;,
 -6.21165;7.84875;9.70597;,
 -8.78460;14.06040;9.70597;,
 -6.21165;20.27205;9.70597;,
 0.00000;22.84500;9.70596;,
 6.21165;20.27205;9.70597;,
 8.78460;14.06040;9.70597;,
 6.21165;7.84875;9.70597;,
 0.00000;5.27580;9.70597;,
 0.00000;8.78964;7.41802;,
 -3.72699;10.33341;7.41802;,
 -5.27076;14.06039;7.41802;,
 -3.72699;17.78739;7.41802;,
 0.00000;19.33116;7.41802;,
 3.72699;17.78739;7.41802;,
 5.27076;14.06039;7.41802;,
 3.72699;10.33341;7.41802;,
 0.00000;8.78964;7.41802;,
 0.00000;11.92573;5.90390;,
 -1.50943;12.55097;5.90390;,
 -2.13466;14.06039;5.90390;,
 -1.50943;15.56982;5.90390;,
 0.00000;16.19505;5.90390;,
 1.50943;15.56982;5.90390;,
 2.13466;14.06039;5.90390;,
 1.50943;12.55097;5.90390;,
 0.00000;11.92573;5.90390;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06039;5.03639;,
 0.00000;14.06040;11.53762;,
 0.00000;0.75039;14.06039;,
 -9.41159;4.64881;14.06039;,
 -13.31000;14.06040;14.06039;,
 -9.41159;23.47199;14.06039;,
 0.00000;27.37040;14.06039;,
 9.41159;23.47199;14.06039;,
 13.31000;14.06040;14.06039;,
 9.41159;4.64881;14.06039;;
 
 112;
 3;2,1,0;,
 3;1,4,3;,
 3;4,6,5;,
 3;6,8,7;,
 3;8,10,9;,
 3;10,12,11;,
 3;12,14,13;,
 3;14,16,15;,
 4;18,17,1,2;,
 4;17,19,4,1;,
 4;19,20,6,4;,
 4;20,21,8,6;,
 4;21,22,10,8;,
 4;22,23,12,10;,
 4;23,24,14,12;,
 4;24,25,16,14;,
 4;27,26,17,18;,
 4;26,28,19,17;,
 4;28,29,20,19;,
 4;29,30,21,20;,
 4;30,31,22,21;,
 4;31,32,23,22;,
 4;32,33,24,23;,
 4;33,34,25,24;,
 4;36,35,26,27;,
 4;35,37,28,26;,
 4;37,38,29,28;,
 4;38,39,30,29;,
 4;39,40,31,30;,
 4;40,41,32,31;,
 4;41,42,33,32;,
 4;42,43,34,33;,
 4;45,44,35,36;,
 4;44,46,37,35;,
 4;46,47,38,37;,
 4;47,48,39,38;,
 4;48,49,40,39;,
 4;49,50,41,40;,
 4;50,51,42,41;,
 4;51,52,43,42;,
 4;54,53,44,45;,
 4;53,55,46,44;,
 4;55,56,47,46;,
 4;56,57,48,47;,
 4;57,58,49,48;,
 4;58,59,50,49;,
 4;59,60,51,50;,
 4;60,61,52,51;,
 4;63,62,53,54;,
 4;62,64,55,53;,
 4;64,65,56,55;,
 4;65,66,57,56;,
 4;66,67,58,57;,
 4;67,68,59,58;,
 4;68,69,60,59;,
 4;69,70,61,60;,
 3;71,62,63;,
 3;72,64,62;,
 3;73,65,64;,
 3;74,66,65;,
 3;75,67,66;,
 3;76,68,67;,
 3;77,69,68;,
 3;78,70,69;,
 4;82,81,80,79;,
 4;84,83,81,82;,
 4;86,85,83,84;,
 4;88,87,85,86;,
 4;90,89,87,88;,
 4;92,91,89,90;,
 4;94,93,91,92;,
 4;96,95,93,94;,
 4;81,98,97,80;,
 4;83,99,98,81;,
 4;85,100,99,83;,
 4;87,101,100,85;,
 4;89,102,101,87;,
 4;91,103,102,89;,
 4;93,104,103,91;,
 4;95,105,104,93;,
 4;98,107,106,97;,
 4;99,108,107,98;,
 4;100,109,108,99;,
 4;101,110,109,100;,
 4;102,111,110,101;,
 4;103,112,111,102;,
 4;104,113,112,103;,
 4;105,114,113,104;,
 4;107,116,115,106;,
 4;108,117,116,107;,
 4;109,118,117,108;,
 4;110,119,118,109;,
 4;111,120,119,110;,
 4;112,121,120,111;,
 4;113,122,121,112;,
 4;114,123,122,113;,
 3;116,124,115;,
 3;117,125,116;,
 3;118,126,117;,
 3;119,127,118;,
 3;120,128,119;,
 3;121,129,120;,
 3;122,130,121;,
 3;123,131,122;,
 3;134,133,132;,
 3;135,134,132;,
 3;136,135,132;,
 3;137,136,132;,
 3;138,137,132;,
 3;139,138,132;,
 3;140,139,132;,
 3;133,140,132;;
 
 MeshMaterialList {
  2;
  112;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
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
 }
 MeshNormals {
  108;
  -0.000000;-0.000000;1.000000;,
  0.000000;-0.380611;0.924735;,
  0.269132;-0.269132;0.924735;,
  0.380611;-0.000000;0.924735;,
  0.269132;0.269132;0.924735;,
  0.000000;0.380611;0.924735;,
  -0.269132;0.269132;0.924735;,
  -0.380611;-0.000000;0.924735;,
  -0.269132;-0.269132;0.924735;,
  0.000000;-0.704977;0.709230;,
  0.498494;-0.498494;0.709230;,
  0.704977;0.000000;0.709230;,
  0.498494;0.498494;0.709230;,
  0.000000;0.704977;0.709230;,
  -0.498494;0.498494;0.709231;,
  -0.704976;-0.000000;0.709231;,
  -0.498494;-0.498494;0.709231;,
  0.000000;-0.923104;0.384551;,
  0.652733;-0.652733;0.384551;,
  0.923104;0.000000;0.384551;,
  0.652733;0.652733;0.384551;,
  0.000000;0.923104;0.384551;,
  -0.652733;0.652733;0.384551;,
  -0.923104;0.000000;0.384550;,
  -0.652733;-0.652733;0.384551;,
  0.000000;-1.000000;0.000000;,
  0.707107;-0.707107;0.000000;,
  1.000000;-0.000000;0.000000;,
  0.707107;0.707107;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.707107;0.707107;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.707107;-0.707107;0.000000;,
  0.000000;-0.923104;-0.384551;,
  0.652733;-0.652733;-0.384551;,
  0.923104;-0.000000;-0.384551;,
  0.652733;0.652733;-0.384551;,
  0.000000;0.923104;-0.384551;,
  -0.652733;0.652733;-0.384551;,
  -0.923104;-0.000000;-0.384551;,
  -0.652733;-0.652733;-0.384551;,
  0.000000;-0.704976;-0.709231;,
  0.498494;-0.498494;-0.709231;,
  0.704976;-0.000000;-0.709231;,
  0.498493;0.498493;-0.709231;,
  0.000000;0.704976;-0.709231;,
  -0.498493;0.498493;-0.709231;,
  -0.704976;-0.000000;-0.709231;,
  -0.498493;-0.498493;-0.709231;,
  0.000000;-0.380611;-0.924735;,
  0.269132;-0.269132;-0.924735;,
  0.380611;-0.000000;-0.924735;,
  0.269132;0.269132;-0.924735;,
  0.000000;0.380611;-0.924735;,
  -0.269132;0.269132;-0.924735;,
  -0.380611;-0.000000;-0.924735;,
  -0.269132;-0.269132;-0.924735;,
  -0.000000;0.000000;-1.000000;,
  0.000000;-0.754015;-0.656858;,
  -0.533169;-0.533169;-0.656857;,
  -0.754015;-0.000000;-0.656857;,
  -0.533169;0.533169;-0.656857;,
  0.000000;0.754016;-0.656857;,
  0.533169;0.533169;-0.656857;,
  0.754015;-0.000000;-0.656857;,
  0.533169;-0.533169;-0.656857;,
  0.000000;-0.704753;-0.709453;,
  -0.498335;-0.498335;-0.709453;,
  -0.704753;-0.000000;-0.709453;,
  -0.498336;0.498336;-0.709453;,
  0.000000;0.704753;-0.709453;,
  0.498336;0.498336;-0.709453;,
  0.704753;-0.000000;-0.709453;,
  0.498335;-0.498335;-0.709453;,
  0.000000;-0.600112;-0.799916;,
  -0.424343;-0.424343;-0.799916;,
  -0.600111;-0.000000;-0.799916;,
  -0.424343;0.424343;-0.799916;,
  0.000000;0.600111;-0.799917;,
  0.424343;0.424343;-0.799916;,
  0.600111;-0.000000;-0.799916;,
  0.424343;-0.424343;-0.799916;,
  0.000000;-0.490964;-0.871180;,
  -0.347164;-0.347164;-0.871180;,
  -0.490963;-0.000000;-0.871180;,
  -0.347164;0.347164;-0.871180;,
  0.000000;0.490963;-0.871180;,
  0.347164;0.347164;-0.871180;,
  0.490963;-0.000000;-0.871180;,
  0.347164;-0.347164;-0.871180;,
  0.000000;-0.405785;-0.913969;,
  -0.286934;-0.286934;-0.913968;,
  -0.405785;-0.000000;-0.913969;,
  -0.286933;0.286933;-0.913969;,
  0.000000;0.405785;-0.913969;,
  0.286933;0.286933;-0.913969;,
  0.405785;-0.000000;-0.913969;,
  0.286934;-0.286934;-0.913968;,
  0.000000;-0.000000;-1.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.186224;0.982507;,
  0.131681;0.131681;0.982507;,
  0.186224;0.000000;0.982507;,
  0.131681;-0.131681;0.982507;,
  0.000000;-0.186224;0.982507;,
  -0.131681;-0.131681;0.982507;,
  -0.186224;0.000000;0.982507;,
  -0.131681;0.131681;0.982507;;
  112;
  3;1,2,0;,
  3;2,3,0;,
  3;3,4,0;,
  3;4,5,0;,
  3;5,6,0;,
  3;6,7,0;,
  3;7,8,0;,
  3;8,1,0;,
  4;9,10,2,1;,
  4;10,11,3,2;,
  4;11,12,4,3;,
  4;12,13,5,4;,
  4;13,14,6,5;,
  4;14,15,7,6;,
  4;15,16,8,7;,
  4;16,9,1,8;,
  4;17,18,10,9;,
  4;18,19,11,10;,
  4;19,20,12,11;,
  4;20,21,13,12;,
  4;21,22,14,13;,
  4;22,23,15,14;,
  4;23,24,16,15;,
  4;24,17,9,16;,
  4;25,26,18,17;,
  4;26,27,19,18;,
  4;27,28,20,19;,
  4;28,29,21,20;,
  4;29,30,22,21;,
  4;30,31,23,22;,
  4;31,32,24,23;,
  4;32,25,17,24;,
  4;33,34,26,25;,
  4;34,35,27,26;,
  4;35,36,28,27;,
  4;36,37,29,28;,
  4;37,38,30,29;,
  4;38,39,31,30;,
  4;39,40,32,31;,
  4;40,33,25,32;,
  4;41,42,34,33;,
  4;42,43,35,34;,
  4;43,44,36,35;,
  4;44,45,37,36;,
  4;45,46,38,37;,
  4;46,47,39,38;,
  4;47,48,40,39;,
  4;48,41,33,40;,
  4;49,50,42,41;,
  4;50,51,43,42;,
  4;51,52,44,43;,
  4;52,53,45,44;,
  4;53,54,46,45;,
  4;54,55,47,46;,
  4;55,56,48,47;,
  4;56,49,41,48;,
  3;57,50,49;,
  3;57,51,50;,
  3;57,52,51;,
  3;57,53,52;,
  3;57,54,53;,
  3;57,55,54;,
  3;57,56,55;,
  3;57,49,56;,
  4;59,67,66,58;,
  4;60,68,67,59;,
  4;61,69,68,60;,
  4;62,70,69,61;,
  4;63,71,70,62;,
  4;64,72,71,63;,
  4;65,73,72,64;,
  4;58,66,73,65;,
  4;67,75,74,66;,
  4;68,76,75,67;,
  4;69,77,76,68;,
  4;70,78,77,69;,
  4;71,79,78,70;,
  4;72,80,79,71;,
  4;73,81,80,72;,
  4;66,74,81,73;,
  4;75,83,82,74;,
  4;76,84,83,75;,
  4;77,85,84,76;,
  4;78,86,85,77;,
  4;79,87,86,78;,
  4;80,88,87,79;,
  4;81,89,88,80;,
  4;74,82,89,81;,
  4;83,91,90,82;,
  4;84,92,91,83;,
  4;85,93,92,84;,
  4;86,94,93,85;,
  4;87,95,94,86;,
  4;88,96,95,87;,
  4;89,97,96,88;,
  4;82,90,97,89;,
  3;91,98,90;,
  3;92,98,91;,
  3;93,98,92;,
  3;94,98,93;,
  3;95,98,94;,
  3;96,98,95;,
  3;97,98,96;,
  3;90,98,97;,
  3;101,100,99;,
  3;102,101,99;,
  3;103,102,99;,
  3;104,103,99;,
  3;105,104,99;,
  3;106,105,99;,
  3;107,106,99;,
  3;100,107,99;;
 }
 MeshTextureCoords {
  141;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;1.000000;,
  0.000000;0.800000;,
  0.125000;0.800000;,
  0.125000;1.000000;,
  0.250000;0.800000;,
  0.250000;1.000000;,
  0.375000;0.800000;,
  0.375000;1.000000;,
  0.500000;0.800000;,
  0.500000;1.000000;,
  0.625000;0.800000;,
  0.625000;1.000000;,
  0.750000;0.800000;,
  0.750000;1.000000;,
  0.875000;0.800000;,
  0.875000;1.000000;,
  1.000000;0.800000;,
  1.000000;1.000000;,
  0.000000;0.600000;,
  0.125000;0.600000;,
  0.250000;0.600000;,
  0.375000;0.600000;,
  0.500000;0.600000;,
  0.625000;0.600000;,
  0.750000;0.600000;,
  0.875000;0.600000;,
  1.000000;0.600000;,
  0.000000;0.400000;,
  0.125000;0.400000;,
  0.250000;0.400000;,
  0.375000;0.400000;,
  0.500000;0.400000;,
  0.625000;0.400000;,
  0.750000;0.400000;,
  0.875000;0.400000;,
  1.000000;0.400000;,
  0.000000;0.200000;,
  0.125000;0.200000;,
  0.250000;0.200000;,
  0.375000;0.200000;,
  0.500000;0.200000;,
  0.625000;0.200000;,
  0.750000;0.200000;,
  0.875000;0.200000;,
  1.000000;0.200000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}