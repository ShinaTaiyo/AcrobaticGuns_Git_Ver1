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
 118;
 1.28236;9.69084;0.00000;,
 1.28236;9.69084;-2.56472;,
 -1.28236;9.69084;-2.56472;,
 -1.28236;9.69084;0.00000;,
 -1.28236;0.00000;0.00000;,
 -1.28236;0.00000;-2.56472;,
 1.28236;0.00000;-2.56472;,
 1.28236;0.00000;0.00000;,
 -1.28236;7.38601;-2.56472;,
 -1.28236;0.00000;-2.56472;,
 -1.28236;0.00000;0.00000;,
 -1.28236;7.38601;0.00000;,
 -1.28236;9.69084;0.00000;,
 -1.28236;9.69084;-2.56472;,
 1.28236;7.38601;-2.56472;,
 -1.28236;0.00000;-2.56472;,
 -1.28236;7.38601;-2.56472;,
 1.28236;7.38601;0.00000;,
 1.28236;0.00000;0.00000;,
 1.28236;0.00000;-2.56472;,
 1.28236;7.38601;-2.56472;,
 1.28236;9.69084;-2.56472;,
 1.28236;9.69084;0.00000;,
 -1.28236;7.38601;0.00000;,
 -1.28236;0.00000;0.00000;,
 1.28236;7.38601;0.00000;,
 -1.28236;9.69084;0.00000;,
 -1.28236;7.50125;-15.09805;,
 1.28236;7.50125;-15.09805;,
 -1.28236;9.57560;-15.09805;,
 -1.28236;9.69084;-2.56472;,
 1.28236;9.57560;-15.09805;,
 -0.88629;9.63282;-5.90570;,
 -0.62670;9.00611;-5.90570;,
 -0.62670;9.00611;-1.30806;,
 -0.88629;9.63282;-1.30806;,
 0.00000;8.74652;-5.90570;,
 0.00000;8.74652;-1.30806;,
 0.62670;9.00611;-5.90570;,
 0.62670;9.00611;-1.30806;,
 0.88629;9.63282;-5.90570;,
 0.88629;9.63282;-1.30806;,
 0.62670;10.25952;-5.90570;,
 0.62670;10.25952;-1.30806;,
 0.00000;10.51911;-5.90570;,
 0.00000;10.51911;-1.30806;,
 -0.62670;10.25952;-5.90570;,
 -0.62670;10.25952;-1.30806;,
 -0.88629;9.63282;-5.90570;,
 -0.88629;9.63282;-1.30806;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-5.90570;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 0.00000;9.63282;-1.30806;,
 -0.64611;7.55602;-5.54823;,
 -0.45687;7.55602;-6.00510;,
 -0.45687;6.11875;-6.00510;,
 -0.64611;6.11875;-5.54823;,
 0.00000;7.55602;-6.19434;,
 0.00000;6.11875;-6.19434;,
 0.45687;7.55602;-6.00510;,
 0.45687;6.11875;-6.00510;,
 0.64611;7.55602;-5.54823;,
 0.64611;6.11875;-5.54823;,
 0.45687;7.55602;-5.09136;,
 0.45687;6.11875;-5.09136;,
 0.00000;7.55602;-4.90213;,
 0.00000;6.11875;-4.90213;,
 -0.45687;7.55602;-5.09136;,
 -0.45687;6.11875;-5.09136;,
 -0.64611;7.55602;-5.54823;,
 -0.64611;6.11875;-5.54823;,
 -0.45687;4.14794;-5.02347;,
 -0.64611;4.47100;-4.70042;,
 0.00000;4.01413;-5.15728;,
 0.45687;4.14794;-5.02347;,
 0.64611;4.47100;-4.70042;,
 0.45687;4.79405;-4.37736;,
 0.00000;4.92786;-4.24355;,
 -0.45687;4.79405;-4.37736;,
 -0.64611;4.47100;-4.70042;,
 -0.45687;3.07530;-2.32813;,
 -0.64611;3.53217;-2.32813;,
 0.00000;2.88606;-2.32813;,
 0.45687;3.07530;-2.32813;,
 0.64611;3.53217;-2.32813;,
 0.45687;3.98904;-2.32813;,
 0.00000;4.17827;-2.32813;,
 -0.45687;3.98904;-2.32813;,
 -0.64611;3.53217;-2.32813;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;7.55602;-5.54823;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;,
 0.00000;3.53217;-2.32813;;
 
 78;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,6,15,16;,
 4;17,18,19,20;,
 4;17,20,21,22;,
 4;23,24,7,25;,
 4;23,25,0,26;,
 4;27,28,14,16;,
 4;29,27,16,30;,
 4;31,29,30,1;,
 4;28,31,1,14;,
 4;28,27,29,31;,
 4;32,33,34,35;,
 4;33,36,37,34;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 3;50,33,32;,
 3;51,36,33;,
 3;52,38,36;,
 3;53,40,38;,
 3;54,42,40;,
 3;55,44,42;,
 3;56,46,44;,
 3;57,48,46;,
 3;58,35,34;,
 3;59,34,37;,
 3;60,37,39;,
 3;61,39,41;,
 3;62,41,43;,
 3;63,43,45;,
 3;64,45,47;,
 3;65,47,49;,
 4;66,67,68,69;,
 4;67,70,71,68;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;69,68,84,85;,
 4;68,71,86,84;,
 4;71,73,87,86;,
 4;73,75,88,87;,
 4;75,77,89,88;,
 4;77,79,90,89;,
 4;79,81,91,90;,
 4;81,83,92,91;,
 4;85,84,93,94;,
 4;84,86,95,93;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;88,89,98,97;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 3;102,67,66;,
 3;103,70,67;,
 3;104,72,70;,
 3;105,74,72;,
 3;106,76,74;,
 3;107,78,76;,
 3;108,80,78;,
 3;109,82,80;,
 3;110,94,93;,
 3;111,93,95;,
 3;112,95,96;,
 3;113,96,97;,
 3;114,97,98;,
 3;115,98,99;,
 3;116,99,100;,
 3;117,100,101;;
 
 MeshMaterialList {
  11;
  78;
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10;;
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
  Material {
   0.000000;0.599200;0.135200;1.000000;;
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
   0.721600;0.401600;0.721600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.398400;0.800000;0.661600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.589600;0.721600;0.404800;1.000000;;
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
 }
 MeshNormals {
  51;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.999990;-0.004597;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.999958;-0.009194;,
  0.000000;-0.999958;-0.009194;,
  -1.000000;0.000003;0.000000;,
  -0.707108;-0.707106;0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.707108;-0.707106;-0.000000;,
  1.000000;0.000003;-0.000000;,
  0.707108;0.707106;-0.000000;,
  -0.000000;1.000000;-0.000000;,
  -0.707107;0.707106;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000001;,
  -0.707107;0.000000;-0.707107;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000001;,
  0.707101;0.000000;0.707113;,
  0.000000;0.000000;1.000000;,
  -0.707101;0.000000;0.707113;,
  -0.999998;0.001604;-0.000779;,
  -0.706596;-0.164467;-0.688239;,
  0.000000;-0.229218;-0.973375;,
  0.706596;-0.164467;-0.688239;,
  0.999998;0.001604;-0.000780;,
  0.707833;0.175516;0.684227;,
  -0.000000;0.248668;0.968589;,
  -0.707833;0.175516;0.684227;,
  -0.999998;0.001699;-0.001018;,
  -0.711221;-0.517437;-0.475840;,
  -0.000000;-0.735093;-0.677966;,
  0.711221;-0.517437;-0.475840;,
  0.999998;0.001698;-0.001018;,
  0.709217;0.527626;0.467570;,
  -0.000000;0.750181;0.661232;,
  -0.709217;0.527626;0.467570;,
  -1.000000;0.000169;-0.000467;,
  -0.687077;-0.675106;-0.268620;,
  -0.000000;-0.929006;-0.370066;,
  0.687077;-0.675106;-0.268620;,
  1.000000;0.000169;-0.000467;,
  0.687088;0.676281;0.265618;,
  -0.000000;0.930994;0.365034;,
  -0.687088;0.676281;0.265618;,
  -1.000000;0.000000;0.000000;;
  78;
  4;0,2,2,0;,
  4;1,1,1,1;,
  4;4,3,3,3;,
  4;4,3,3,50;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;9,9,9,9;,
  4;7,7,4,50;,
  4;8,8,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,10,10,17;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  4;19,20,27,26;,
  4;20,5,28,27;,
  4;5,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,19,26,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,26,34,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,34,42,49;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;;
 }
 MeshTextureCoords {
  118;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.177510;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.177510;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.177510;,
  0.000000;1.000000;,
  0.000000;0.177510;,
  1.000000;0.177510;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.177510;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.177510;,
  1.000000;1.000000;,
  0.000000;0.177510;,
  1.000000;0.000000;,
  0.000000;0.177510;,
  1.000000;0.177510;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.333330;,
  0.000000;0.333330;,
  0.250000;0.000000;,
  0.250000;0.333330;,
  0.375000;0.000000;,
  0.375000;0.333330;,
  0.500000;0.000000;,
  0.500000;0.333330;,
  0.625000;0.000000;,
  0.625000;0.333330;,
  0.750000;0.000000;,
  0.750000;0.333330;,
  0.875000;0.000000;,
  0.875000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}