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
 299;
 0.00000;20.50935;0.00000;,
 2.77489;19.72875;-2.77489;,
 0.00000;19.72875;-3.92429;,
 0.00000;20.50935;0.00000;,
 3.92429;19.72875;0.00000;,
 0.00000;20.50935;0.00000;,
 2.77489;19.72875;2.77489;,
 0.00000;20.50935;0.00000;,
 0.00000;19.72875;3.92429;,
 0.00000;20.50935;0.00000;,
 -2.77489;19.72875;2.77489;,
 0.00000;20.50935;0.00000;,
 -3.92429;19.72875;0.00000;,
 0.00000;20.50935;0.00000;,
 -2.77489;19.72875;-2.77489;,
 0.00000;20.50935;0.00000;,
 0.00000;19.72875;-3.92429;,
 5.12734;17.50582;-5.12733;,
 0.00000;17.50582;-7.25115;,
 7.25115;17.50582;0.00000;,
 5.12734;17.50582;5.12734;,
 0.00000;17.50582;7.25115;,
 -5.12734;17.50582;5.12733;,
 -7.25115;17.50582;0.00000;,
 -5.12734;17.50582;-5.12733;,
 0.00000;17.50582;-7.25115;,
 6.69919;14.17898;-6.69919;,
 0.00000;14.17898;-9.47408;,
 9.47408;14.17898;0.00000;,
 6.69919;14.17898;6.69919;,
 0.00000;14.17898;9.47408;,
 -6.69919;14.17898;6.69919;,
 -9.47408;14.17898;0.00000;,
 -6.69919;14.17898;-6.69919;,
 0.00000;14.17898;-9.47408;,
 7.25115;10.25468;-7.25115;,
 0.00000;10.25468;-10.25467;,
 10.25467;10.25468;0.00000;,
 7.25115;10.25468;7.25115;,
 0.00000;10.25468;10.25467;,
 -7.25115;10.25468;7.25115;,
 -10.25467;10.25468;0.00000;,
 -7.25115;10.25468;-7.25115;,
 0.00000;10.25468;-10.25467;,
 6.69919;6.33038;-6.69919;,
 0.00000;6.33038;-9.47408;,
 9.47408;6.33038;0.00000;,
 6.69919;6.33038;6.69919;,
 0.00000;6.33038;9.47408;,
 -6.69919;6.33038;6.69919;,
 -9.47408;6.33038;0.00000;,
 -6.69919;6.33038;-6.69919;,
 0.00000;6.33038;-9.47408;,
 5.12734;3.00354;-5.12733;,
 0.00000;3.00354;-7.25115;,
 7.25115;3.00354;0.00000;,
 5.12734;3.00354;5.12734;,
 0.00000;3.00354;7.25115;,
 -5.12734;3.00354;5.12733;,
 -7.25115;3.00354;0.00000;,
 -5.12734;3.00354;-5.12733;,
 0.00000;3.00354;-7.25115;,
 2.77489;0.78060;-2.77489;,
 0.00000;0.78060;-3.92429;,
 3.92429;0.78060;0.00000;,
 2.77489;0.78060;2.77489;,
 0.00000;0.78060;3.92429;,
 -2.77490;0.78060;2.77489;,
 -3.92429;0.78060;0.00000;,
 -2.77489;0.78060;-2.77489;,
 0.00000;0.78060;-3.92429;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;23.80436;0.00000;,
 2.27302;23.29284;-2.27302;,
 0.00000;23.29284;-3.21454;,
 0.00000;23.80436;0.00000;,
 3.21454;23.29284;0.00000;,
 0.00000;23.80436;0.00000;,
 2.27302;23.29284;2.27302;,
 0.00000;23.80436;0.00000;,
 0.00000;23.29284;3.21454;,
 0.00000;23.80436;0.00000;,
 -2.27302;23.29284;2.27302;,
 0.00000;23.80436;0.00000;,
 -3.21454;23.29284;0.00000;,
 0.00000;23.80436;0.00000;,
 -2.27302;23.29284;-2.27302;,
 0.00000;23.80436;0.00000;,
 0.00000;23.29284;-3.21454;,
 4.20000;21.83611;-4.20000;,
 0.00000;21.83611;-5.93970;,
 5.93970;21.83611;0.00000;,
 4.20000;21.83611;4.20000;,
 0.00000;21.83611;5.93970;,
 -4.20000;21.83611;4.20000;,
 -5.93970;21.83611;0.00000;,
 -4.20000;21.83611;-4.20000;,
 0.00000;21.83611;-5.93970;,
 5.48756;19.65600;-5.48756;,
 0.00000;19.65600;-7.76059;,
 7.76059;19.65600;0.00000;,
 5.48756;19.65600;5.48756;,
 0.00000;19.65600;7.76059;,
 -5.48757;19.65600;5.48756;,
 -7.76059;19.65600;0.00000;,
 -5.48757;19.65600;-5.48756;,
 0.00000;19.65600;-7.76059;,
 10.26380;17.68437;-10.26380;,
 0.00000;17.68437;-14.51520;,
 14.51520;17.68437;0.00000;,
 10.26380;17.68437;10.26380;,
 0.00000;17.68437;14.51520;,
 -10.26380;17.68437;10.26380;,
 -14.51520;17.68437;0.00000;,
 -10.26380;17.68437;-10.26380;,
 0.00000;17.68437;-14.51520;,
 -3.87723;17.08938;-8.82863;,
 -3.28303;16.92223;-9.14441;,
 -3.87723;16.92223;-9.27521;,
 -3.87723;17.08938;-8.82863;,
 -3.03691;16.92223;-8.82863;,
 -3.87723;17.08938;-8.82863;,
 -3.28303;16.92223;-8.51284;,
 -3.87723;17.08938;-8.82863;,
 -3.87723;16.92223;-8.38205;,
 -3.87723;17.08938;-8.82863;,
 -4.47143;16.92223;-8.51285;,
 -3.87723;17.08938;-8.82863;,
 -4.71756;16.92223;-8.82863;,
 -3.87723;17.08938;-8.82863;,
 -4.47143;16.92223;-9.14441;,
 -3.87723;17.08938;-8.82863;,
 -3.87723;16.92223;-9.27521;,
 -2.77929;16.44623;-9.41212;,
 -3.87723;16.44623;-9.65381;,
 -2.32451;16.44623;-8.82863;,
 -2.77929;16.44623;-8.24514;,
 -3.87723;16.44623;-8.00345;,
 -4.97517;16.44623;-8.24514;,
 -5.42995;16.44623;-8.82863;,
 -4.97517;16.44623;-9.41212;,
 -3.87723;16.44623;-9.65381;,
 -2.44271;15.73383;-9.59100;,
 -3.87723;15.73383;-9.90678;,
 -1.84850;15.73383;-8.82863;,
 -2.44271;15.73383;-8.06626;,
 -3.87723;15.73383;-7.75048;,
 -5.31176;15.73383;-8.06626;,
 -5.90596;15.73383;-8.82863;,
 -5.31176;15.73383;-9.59100;,
 -3.87723;15.73383;-9.90678;,
 -2.32451;14.89351;-9.65381;,
 -3.87723;14.89351;-9.99561;,
 -1.68135;14.89351;-8.82863;,
 -2.32451;14.89351;-8.00345;,
 -3.87723;14.89351;-7.66165;,
 -5.42995;14.89351;-8.00345;,
 -6.07311;14.89351;-8.82863;,
 -5.42995;14.89351;-9.65381;,
 -3.87723;14.89351;-9.99561;,
 -2.44271;14.05318;-9.59100;,
 -3.87723;14.05318;-9.90678;,
 -1.84850;14.05318;-8.82863;,
 -2.44271;14.05318;-8.06626;,
 -3.87723;14.05318;-7.75048;,
 -5.31176;14.05318;-8.06626;,
 -5.90596;14.05318;-8.82863;,
 -5.31176;14.05318;-9.59100;,
 -3.87723;14.05318;-9.90678;,
 -2.77929;13.34078;-9.41212;,
 -3.87723;13.34078;-9.65381;,
 -2.32451;13.34078;-8.82863;,
 -2.77929;13.34078;-8.24514;,
 -3.87723;13.34078;-8.00345;,
 -4.97517;13.34078;-8.24514;,
 -5.42995;13.34078;-8.82863;,
 -4.97517;13.34078;-9.41212;,
 -3.87723;13.34078;-9.65381;,
 -3.28303;12.86478;-9.14441;,
 -3.87723;12.86478;-9.27521;,
 -3.03691;12.86478;-8.82863;,
 -3.28303;12.86478;-8.51284;,
 -3.87723;12.86478;-8.38205;,
 -4.47143;12.86478;-8.51284;,
 -4.71756;12.86478;-8.82863;,
 -4.47143;12.86478;-9.14441;,
 -3.87723;12.86478;-9.27521;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 -3.87723;12.69763;-8.82863;,
 3.87723;17.08938;-8.82863;,
 3.28303;16.92223;-8.51284;,
 3.87723;16.92223;-8.38205;,
 3.87723;17.08938;-8.82863;,
 4.47143;16.92223;-8.51285;,
 3.03691;16.92223;-8.82863;,
 2.32451;16.44623;-8.82863;,
 2.77929;16.44623;-8.24514;,
 3.87723;16.44623;-8.00345;,
 4.97517;16.44623;-8.24514;,
 5.42995;16.44623;-8.82863;,
 4.71756;16.92223;-8.82863;,
 1.84850;15.73383;-8.82863;,
 2.44271;15.73383;-8.06626;,
 3.87723;15.73383;-7.75048;,
 5.31176;15.73383;-8.06626;,
 5.90596;15.73383;-8.82863;,
 1.68135;14.89351;-8.82863;,
 2.32451;14.89351;-8.00345;,
 3.87723;14.89351;-7.66165;,
 5.42995;14.89351;-8.00345;,
 6.07311;14.89351;-8.82863;,
 1.84850;14.05318;-8.82863;,
 2.44271;14.05318;-8.06626;,
 3.87723;14.05318;-7.75048;,
 5.31176;14.05318;-8.06626;,
 5.90596;14.05318;-8.82863;,
 2.32451;13.34078;-8.82863;,
 2.77929;13.34078;-8.24514;,
 3.87723;13.34078;-8.00345;,
 4.97517;13.34078;-8.24514;,
 5.42995;13.34078;-8.82863;,
 3.03691;12.86478;-8.82863;,
 3.28303;12.86478;-8.51284;,
 3.87723;12.86478;-8.38205;,
 4.47143;12.86478;-8.51284;,
 4.71756;12.86478;-8.82863;,
 3.28303;12.86478;-9.14441;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;,
 4.47143;12.86478;-9.14441;,
 3.87723;17.08938;-8.82863;,
 3.87723;16.92223;-9.27521;,
 3.28303;16.92223;-9.14441;,
 3.87723;17.08938;-8.82863;,
 3.03691;16.92223;-8.82863;,
 3.87723;17.08938;-8.82863;,
 3.28303;16.92223;-8.51284;,
 3.87723;17.08938;-8.82863;,
 4.47143;16.92223;-8.51285;,
 4.71756;16.92223;-8.82863;,
 3.87723;17.08938;-8.82863;,
 4.47143;16.92223;-9.14441;,
 3.87723;17.08938;-8.82863;,
 3.87723;16.92223;-9.27521;,
 3.87723;16.44623;-9.65381;,
 2.77929;16.44623;-9.41212;,
 2.32451;16.44623;-8.82863;,
 5.42995;16.44623;-8.82863;,
 4.97517;16.44623;-9.41212;,
 3.87723;16.44623;-9.65381;,
 3.87723;15.73383;-9.90678;,
 2.44271;15.73383;-9.59100;,
 1.84850;15.73383;-8.82863;,
 5.90596;15.73383;-8.82863;,
 5.31176;15.73383;-9.59100;,
 3.87723;15.73383;-9.90678;,
 3.87723;14.89351;-9.99561;,
 2.32451;14.89351;-9.65381;,
 1.68135;14.89351;-8.82863;,
 6.07311;14.89351;-8.82863;,
 5.42995;14.89351;-9.65381;,
 3.87723;14.89351;-9.99561;,
 3.87723;14.05318;-9.90678;,
 2.44271;14.05318;-9.59100;,
 1.84850;14.05318;-8.82863;,
 5.90596;14.05318;-8.82863;,
 5.31176;14.05318;-9.59100;,
 3.87723;14.05318;-9.90678;,
 3.87723;13.34078;-9.65381;,
 2.77929;13.34078;-9.41212;,
 2.32451;13.34078;-8.82863;,
 5.42995;13.34078;-8.82863;,
 4.97517;13.34078;-9.41212;,
 3.87723;13.34078;-9.65381;,
 3.87723;12.86478;-9.27521;,
 3.28303;12.86478;-9.14441;,
 3.03691;12.86478;-8.82863;,
 4.71756;12.86478;-8.82863;,
 4.47143;12.86478;-9.14441;,
 3.87723;12.86478;-9.27521;,
 3.87723;12.69763;-8.82863;,
 3.87723;12.69763;-8.82863;;
 
 224;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 3;79,80,81;,
 3;82,83,80;,
 3;84,85,83;,
 3;86,87,85;,
 3;88,89,87;,
 3;90,91,89;,
 3;92,93,91;,
 3;94,95,93;,
 4;81,80,96,97;,
 4;80,83,98,96;,
 4;83,85,99,98;,
 4;85,87,100,99;,
 4;87,89,101,100;,
 4;89,91,102,101;,
 4;91,93,103,102;,
 4;93,95,104,103;,
 4;97,96,105,106;,
 4;96,98,107,105;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;101,102,111,110;,
 4;102,103,112,111;,
 4;103,104,113,112;,
 4;106,105,114,115;,
 4;105,107,116,114;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;110,111,120,119;,
 4;111,112,121,120;,
 4;112,113,122,121;,
 3;123,124,125;,
 3;126,127,124;,
 3;128,129,127;,
 3;130,131,129;,
 3;132,133,131;,
 3;134,135,133;,
 3;136,137,135;,
 3;138,139,137;,
 4;125,124,140,141;,
 4;124,127,142,140;,
 4;127,129,143,142;,
 4;129,131,144,143;,
 4;131,133,145,144;,
 4;133,135,146,145;,
 4;135,137,147,146;,
 4;137,139,148,147;,
 4;141,140,149,150;,
 4;140,142,151,149;,
 4;142,143,152,151;,
 4;143,144,153,152;,
 4;144,145,154,153;,
 4;145,146,155,154;,
 4;146,147,156,155;,
 4;147,148,157,156;,
 4;150,149,158,159;,
 4;149,151,160,158;,
 4;151,152,161,160;,
 4;152,153,162,161;,
 4;153,154,163,162;,
 4;154,155,164,163;,
 4;155,156,165,164;,
 4;156,157,166,165;,
 4;159,158,167,168;,
 4;158,160,169,167;,
 4;160,161,170,169;,
 4;161,162,171,170;,
 4;162,163,172,171;,
 4;163,164,173,172;,
 4;164,165,174,173;,
 4;165,166,175,174;,
 4;168,167,176,177;,
 4;167,169,178,176;,
 4;169,170,179,178;,
 4;170,171,180,179;,
 4;171,172,181,180;,
 4;172,173,182,181;,
 4;173,174,183,182;,
 4;174,175,184,183;,
 4;177,176,185,186;,
 4;176,178,187,185;,
 4;178,179,188,187;,
 4;179,180,189,188;,
 4;180,181,190,189;,
 4;181,182,191,190;,
 4;182,183,192,191;,
 4;183,184,193,192;,
 3;186,185,194;,
 3;185,187,195;,
 3;187,188,196;,
 3;188,189,197;,
 3;189,190,198;,
 3;190,191,199;,
 3;191,192,200;,
 3;192,193,201;,
 3;202,203,204;,
 3;205,204,206;,
 4;207,208,209,203;,
 4;203,209,210,204;,
 4;204,210,211,206;,
 4;206,211,212,213;,
 4;208,214,215,209;,
 4;209,215,216,210;,
 4;210,216,217,211;,
 4;211,217,218,212;,
 4;214,219,220,215;,
 4;215,220,221,216;,
 4;216,221,222,217;,
 4;217,222,223,218;,
 4;219,224,225,220;,
 4;220,225,226,221;,
 4;221,226,227,222;,
 4;222,227,228,223;,
 4;224,229,230,225;,
 4;225,230,231,226;,
 4;226,231,232,227;,
 4;227,232,233,228;,
 4;229,234,235,230;,
 4;230,235,236,231;,
 4;231,236,237,232;,
 4;232,237,238,233;,
 3;239,240,234;,
 3;234,241,235;,
 3;235,242,236;,
 3;236,243,237;,
 3;237,244,238;,
 3;238,245,246;,
 3;247,248,249;,
 3;250,249,251;,
 3;252,251,253;,
 3;254,255,256;,
 3;257,256,258;,
 3;259,258,260;,
 4;248,261,262,249;,
 4;249,262,263,251;,
 4;256,264,265,258;,
 4;258,265,266,260;,
 4;261,267,268,262;,
 4;262,268,269,263;,
 4;264,270,271,265;,
 4;265,271,272,266;,
 4;267,273,274,268;,
 4;268,274,275,269;,
 4;270,276,277,271;,
 4;271,277,278,272;,
 4;273,279,280,274;,
 4;274,280,281,275;,
 4;276,282,283,277;,
 4;277,283,284,278;,
 4;279,285,286,280;,
 4;280,286,287,281;,
 4;282,288,289,283;,
 4;283,289,290,284;,
 4;285,291,292,286;,
 4;286,292,293,287;,
 4;288,294,295,289;,
 4;289,295,296,290;,
 3;291,297,292;,
 3;295,298,296;;
 
 MeshMaterialList {
  11;
  224;
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
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
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
  235;
  0.000000;1.000000;0.000000;,
  0.000000;0.924735;-0.380611;,
  0.269133;0.924735;-0.269133;,
  0.380611;0.924735;-0.000000;,
  0.269133;0.924735;0.269133;,
  -0.000000;0.924735;0.380611;,
  -0.269133;0.924735;0.269133;,
  -0.380611;0.924735;0.000000;,
  -0.269133;0.924735;-0.269133;,
  0.000000;0.709232;-0.704976;,
  0.498493;0.709231;-0.498493;,
  0.704976;0.709231;-0.000000;,
  0.498493;0.709232;0.498493;,
  -0.000000;0.709232;0.704976;,
  -0.498493;0.709231;0.498493;,
  -0.704976;0.709232;0.000000;,
  -0.498493;0.709232;-0.498493;,
  0.000000;0.384552;-0.923103;,
  0.652733;0.384552;-0.652733;,
  0.923104;0.384551;-0.000000;,
  0.652733;0.384551;0.652733;,
  -0.000000;0.384551;0.923104;,
  -0.652733;0.384552;0.652733;,
  -0.923104;0.384552;0.000000;,
  -0.652733;0.384552;-0.652733;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.000000;-0.384552;-0.923103;,
  0.652733;-0.384552;-0.652733;,
  0.923104;-0.384551;-0.000000;,
  0.652733;-0.384551;0.652733;,
  -0.000000;-0.384551;0.923104;,
  -0.652733;-0.384552;0.652733;,
  -0.923104;-0.384551;0.000000;,
  -0.652733;-0.384552;-0.652733;,
  0.000000;-0.709231;-0.704976;,
  0.498494;-0.709231;-0.498493;,
  0.704976;-0.709231;-0.000000;,
  0.498493;-0.709231;0.498493;,
  -0.000000;-0.709231;0.704976;,
  -0.498494;-0.709231;0.498493;,
  -0.704976;-0.709231;-0.000000;,
  -0.498494;-0.709231;-0.498493;,
  0.000000;-0.924735;-0.380612;,
  0.269134;-0.924735;-0.269134;,
  0.380612;-0.924735;-0.000000;,
  0.269133;-0.924735;0.269133;,
  -0.000000;-0.924735;0.380612;,
  -0.269134;-0.924735;0.269133;,
  -0.380612;-0.924735;-0.000000;,
  -0.269134;-0.924735;-0.269134;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.948301;-0.317373;,
  0.224417;0.948301;-0.224417;,
  0.317373;0.948301;0.000000;,
  0.224417;0.948301;0.224417;,
  0.000000;0.948301;0.317373;,
  -0.224417;0.948301;0.224417;,
  -0.317373;0.948301;0.000000;,
  -0.224417;0.948301;-0.224417;,
  0.000000;0.777534;-0.628841;,
  0.444658;0.777534;-0.444658;,
  0.628841;0.777534;0.000000;,
  0.444658;0.777534;0.444658;,
  0.000000;0.777534;0.628841;,
  -0.444657;0.777534;0.444657;,
  -0.628840;0.777534;0.000000;,
  -0.444657;0.777534;-0.444657;,
  0.000000;0.840189;-0.542294;,
  0.383460;0.840189;-0.383460;,
  0.542294;0.840189;0.000000;,
  0.383460;0.840189;0.383460;,
  0.000000;0.840189;0.542294;,
  -0.383460;0.840189;0.383460;,
  -0.542294;0.840189;0.000000;,
  -0.383460;0.840189;-0.383460;,
  0.000000;0.959942;-0.280201;,
  0.198132;0.959941;-0.198132;,
  0.280200;0.959941;0.000000;,
  0.198132;0.959941;0.198132;,
  0.000000;0.959942;0.280201;,
  -0.198132;0.959941;0.198132;,
  -0.280201;0.959941;0.000000;,
  -0.198132;0.959941;-0.198132;,
  -0.000000;1.000000;-0.000001;,
  -0.000000;0.809851;-0.586635;,
  0.249820;0.863045;-0.439025;,
  0.375827;0.926690;-0.000002;,
  0.249818;0.863046;0.439024;,
  -0.000003;0.809851;0.586636;,
  -0.249820;0.863044;0.439028;,
  -0.375826;0.926690;0.000000;,
  -0.249820;0.863044;-0.439028;,
  0.000000;0.485643;-0.874157;,
  0.420118;0.576914;-0.700479;,
  0.700795;0.713363;-0.000001;,
  0.420118;0.576914;0.700479;,
  -0.000001;0.485642;0.874158;,
  -0.420118;0.576914;0.700479;,
  -0.700794;0.713363;0.000000;,
  -0.420118;0.576914;-0.700479;,
  0.000000;0.221548;-0.975150;,
  0.506772;0.281376;-0.814868;,
  0.748606;0.314869;-0.583478;,
  0.506772;0.281376;0.814868;,
  0.000000;0.221548;0.975150;,
  -0.506774;0.281376;0.814867;,
  -0.748608;0.314870;0.583475;,
  -0.506774;0.281376;-0.814867;,
  0.000000;0.000001;-1.000000;,
  0.532428;0.000001;-0.846476;,
  0.788724;0.000001;-0.614748;,
  0.532428;0.000001;0.846476;,
  0.000000;0.000001;1.000000;,
  -0.532429;0.000001;0.846475;,
  -0.788726;0.000001;0.614745;,
  -0.532429;0.000001;-0.846475;,
  0.000000;-0.221547;-0.975150;,
  0.506773;-0.281375;-0.814868;,
  0.748606;-0.314868;-0.583478;,
  0.506773;-0.281375;0.814868;,
  0.000000;-0.221547;0.975150;,
  -0.506774;-0.281375;0.814867;,
  -0.748608;-0.314869;0.583475;,
  -0.506774;-0.281375;-0.814867;,
  0.000000;-0.485643;-0.874157;,
  0.420118;-0.576914;-0.700479;,
  0.639403;-0.741731;-0.202480;,
  0.420118;-0.576914;0.700479;,
  0.000000;-0.485641;0.874158;,
  -0.420118;-0.576914;0.700480;,
  -0.639405;-0.741731;0.202476;,
  -0.420118;-0.576914;-0.700479;,
  -0.000000;-0.809852;-0.586635;,
  0.249820;-0.863045;-0.439025;,
  0.375827;-0.926690;-0.000002;,
  0.249818;-0.863046;0.439024;,
  -0.000000;-0.809850;0.586637;,
  -0.249818;-0.863045;0.439028;,
  -0.375827;-0.926690;-0.000002;,
  -0.249820;-0.863044;-0.439028;,
  0.000001;-1.000000;-0.000001;,
  0.000003;0.936549;0.350536;,
  -0.509823;0.763014;0.397354;,
  -0.263961;0.806070;0.529694;,
  0.000003;0.809851;0.586636;,
  0.263964;0.806068;0.529696;,
  0.509821;0.763009;0.397367;,
  -0.615031;0.626060;0.479361;,
  -0.420118;0.576914;0.700479;,
  0.000001;0.485642;0.874158;,
  0.420118;0.576914;0.700479;,
  0.615029;0.626059;0.479364;,
  -0.748606;0.314869;0.583478;,
  -0.506772;0.281376;0.814868;,
  -0.000000;0.221548;0.975150;,
  0.506774;0.281376;0.814867;,
  0.748608;0.314870;0.583475;,
  -0.788724;0.000001;0.614748;,
  -0.532428;0.000001;0.846476;,
  -0.000000;0.000001;1.000000;,
  0.532429;0.000001;0.846475;,
  0.788726;0.000001;0.614745;,
  -0.748606;-0.314868;0.583478;,
  -0.506773;-0.281375;0.814868;,
  -0.000000;-0.221547;0.975150;,
  0.506774;-0.281375;0.814867;,
  0.748608;-0.314869;0.583475;,
  -0.615031;-0.626060;0.479361;,
  -0.420118;-0.576914;0.700479;,
  -0.000000;-0.485641;0.874158;,
  0.420118;-0.576914;0.700480;,
  0.615030;-0.626059;0.479362;,
  -0.192871;-0.969641;-0.150324;,
  -0.311563;-0.940117;0.138236;,
  -0.249818;-0.863046;0.439024;,
  0.000000;-0.809850;0.586637;,
  0.249818;-0.863045;0.439028;,
  0.311562;-0.940116;0.138239;,
  0.192869;-0.969641;-0.150329;,
  -0.000001;-0.992682;0.120755;,
  -0.000001;0.992682;-0.120757;,
  0.000000;0.809851;-0.586635;,
  -0.249820;0.863045;-0.439025;,
  -0.311562;0.940116;-0.138241;,
  -0.192871;0.969642;0.150320;,
  0.192869;0.969641;0.150329;,
  0.311561;0.940116;-0.138242;,
  0.249820;0.863044;-0.439028;,
  -0.000000;0.485643;-0.874157;,
  -0.420118;0.576914;-0.700479;,
  -0.615029;0.626059;-0.479363;,
  0.615029;0.626059;-0.479364;,
  0.420118;0.576914;-0.700479;,
  -0.000000;0.221548;-0.975150;,
  -0.506772;0.281376;-0.814868;,
  -0.748606;0.314869;-0.583478;,
  0.748608;0.314870;-0.583475;,
  0.506774;0.281376;-0.814867;,
  -0.000000;0.000001;-1.000000;,
  -0.532428;0.000001;-0.846476;,
  -0.788724;0.000001;-0.614748;,
  0.788726;0.000001;-0.614745;,
  0.532429;0.000001;-0.846475;,
  -0.000000;-0.221547;-0.975150;,
  -0.506773;-0.281375;-0.814868;,
  -0.748606;-0.314868;-0.583478;,
  0.748608;-0.314869;-0.583475;,
  0.506774;-0.281375;-0.814867;,
  -0.000000;-0.485643;-0.874157;,
  -0.420118;-0.576914;-0.700479;,
  -0.615029;-0.626059;-0.479363;,
  0.615029;-0.626059;-0.479364;,
  0.420118;-0.576914;-0.700479;,
  0.000000;-0.809852;-0.586635;,
  -0.263964;-0.806070;-0.529693;,
  -0.509821;-0.763013;-0.397359;,
  0.509821;-0.763009;-0.397367;,
  0.263964;-0.806068;-0.529696;,
  0.000000;-0.936549;-0.350536;,
  0.748606;0.314869;0.583478;,
  -0.748608;0.314870;-0.583475;,
  0.788724;0.000001;0.614748;,
  -0.788726;0.000001;-0.614745;,
  0.748606;-0.314868;0.583478;,
  -0.748608;-0.314869;-0.583475;,
  0.697759;-0.466223;0.543846;,
  -0.697759;-0.466228;-0.543842;;
  224;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  3;58,60,59;,
  3;58,61,60;,
  3;58,62,61;,
  3;58,63,62;,
  3;58,64,63;,
  3;58,65,64;,
  3;58,66,65;,
  3;58,59,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,59,67,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,67,75,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,82,90,89;,
  4;82,75,83,90;,
  3;91,93,92;,
  3;91,94,93;,
  3;91,95,94;,
  3;91,96,95;,
  3;91,97,96;,
  3;91,98,97;,
  3;91,99,98;,
  3;91,92,99;,
  4;92,93,101,100;,
  4;93,94,102,101;,
  4;94,95,103,102;,
  4;95,96,104,103;,
  4;96,97,105,104;,
  4;97,98,106,105;,
  4;98,99,107,106;,
  4;99,92,100,107;,
  4;100,101,109,108;,
  4;101,102,110,109;,
  4;102,103,111,227;,
  4;103,104,112,111;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,107,115,228;,
  4;107,100,108,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;227,111,119,229;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;228,115,123,230;,
  4;115,108,116,123;,
  4;116,117,125,124;,
  4;117,118,126,125;,
  4;229,119,127,231;,
  4;119,120,128,127;,
  4;120,121,129,128;,
  4;121,122,130,129;,
  4;230,123,131,232;,
  4;123,116,124,131;,
  4;124,125,133,132;,
  4;125,126,134,133;,
  4;231,127,135,233;,
  4;127,128,136,135;,
  4;128,129,137,136;,
  4;129,130,138,137;,
  4;232,131,139,234;,
  4;131,124,132,139;,
  4;132,133,141,140;,
  4;133,134,142,141;,
  4;134,135,143,142;,
  4;135,136,144,143;,
  4;136,137,145,144;,
  4;137,138,146,145;,
  4;138,139,147,146;,
  4;139,132,140,147;,
  3;140,141,148;,
  3;141,142,148;,
  3;142,143,148;,
  3;143,144,148;,
  3;144,145,148;,
  3;145,146,148;,
  3;146,147,148;,
  3;147,140,148;,
  3;149,151,152;,
  3;149,152,153;,
  4;150,155,156,151;,
  4;151,156,157,152;,
  4;152,157,158,153;,
  4;153,158,159,154;,
  4;155,160,161,156;,
  4;156,161,162,157;,
  4;157,162,163,158;,
  4;158,163,164,159;,
  4;160,165,166,161;,
  4;161,166,167,162;,
  4;162,167,168,163;,
  4;163,168,169,164;,
  4;165,170,171,166;,
  4;166,171,172,167;,
  4;167,172,173,168;,
  4;168,173,174,169;,
  4;170,175,176,171;,
  4;171,176,177,172;,
  4;172,177,178,173;,
  4;173,178,179,174;,
  4;175,181,182,176;,
  4;176,182,183,177;,
  4;177,183,184,178;,
  4;178,184,185,179;,
  3;180,187,181;,
  3;181,187,182;,
  3;182,187,183;,
  3;183,187,184;,
  3;184,187,185;,
  3;185,187,186;,
  3;188,189,190;,
  3;188,190,191;,
  3;188,191,192;,
  3;188,193,194;,
  3;188,194,195;,
  3;188,195,189;,
  4;189,196,197,190;,
  4;190,197,198,191;,
  4;194,199,200,195;,
  4;195,200,196,189;,
  4;196,201,202,197;,
  4;197,202,203,198;,
  4;199,204,205,200;,
  4;200,205,201,196;,
  4;201,206,207,202;,
  4;202,207,208,203;,
  4;204,209,210,205;,
  4;205,210,206,201;,
  4;206,211,212,207;,
  4;207,212,213,208;,
  4;209,214,215,210;,
  4;210,215,211,206;,
  4;211,216,217,212;,
  4;212,217,218,213;,
  4;214,219,220,215;,
  4;215,220,216,211;,
  4;216,221,222,217;,
  4;217,222,223,218;,
  4;219,224,225,220;,
  4;220,225,221,216;,
  3;221,226,222;,
  3;225,226,221;;
 }
 MeshTextureCoords {
  299;
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
  0.437500;0.000000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.250000;0.125000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.750000;0.125000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.125000;0.875000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.875000;0.875000;,
  0.062500;0.000000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.687500;0.000000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.250000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.250000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.937500;1.000000;;
 }
}