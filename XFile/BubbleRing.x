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
 289;
 0.74821;0.00000;-0.00000;,
 0.75499;0.00000;-0.03535;,
 0.69534;-0.28802;-0.03535;,
 0.68909;-0.28543;0.00000;,
 0.77337;0.00000;-0.06553;,
 0.71226;-0.29503;-0.06553;,
 0.54685;-0.54685;-0.06553;,
 0.53386;-0.53386;-0.03535;,
 0.52906;-0.52906;0.00000;,
 0.80133;0.00000;-0.08535;,
 0.73802;-0.30570;-0.08535;,
 0.83410;0.00000;-0.09268;,
 0.76820;-0.31820;-0.09268;,
 0.58980;-0.58980;-0.09268;,
 0.56663;-0.56663;-0.08535;,
 0.86687;0.00000;-0.08535;,
 0.79838;-0.33070;-0.08535;,
 0.89483;0.00000;-0.06553;,
 0.82413;-0.34137;-0.06553;,
 0.63274;-0.63274;-0.06553;,
 0.61297;-0.61297;-0.08535;,
 0.91320;0.00000;-0.03535;,
 0.84105;-0.34838;-0.03535;,
 0.91999;0.00000;0.00000;,
 0.84730;-0.35096;0.00000;,
 0.65053;-0.65053;-0.00000;,
 0.64573;-0.64573;-0.03535;,
 0.91320;0.00000;0.03535;,
 0.84105;-0.34838;0.03535;,
 0.89483;0.00000;0.06553;,
 0.82413;-0.34137;0.06553;,
 0.63274;-0.63274;0.06553;,
 0.64573;-0.64573;0.03535;,
 0.86687;0.00000;0.08535;,
 0.79838;-0.33070;0.08535;,
 0.83410;0.00000;0.09268;,
 0.76820;-0.31820;0.09268;,
 0.58980;-0.58980;0.09268;,
 0.61297;-0.61297;0.08535;,
 0.80133;0.00000;0.08535;,
 0.73802;-0.30570;0.08535;,
 0.77337;0.00000;0.06553;,
 0.71226;-0.29503;0.06553;,
 0.54685;-0.54685;0.06553;,
 0.56663;-0.56663;0.08535;,
 0.75499;0.00000;0.03535;,
 0.69534;-0.28802;0.03535;,
 0.74821;0.00000;-0.00000;,
 0.68909;-0.28543;0.00000;,
 0.52906;-0.52906;0.00000;,
 0.53386;-0.53386;0.03535;,
 0.28802;-0.69534;-0.03535;,
 0.28543;-0.68909;0.00000;,
 0.29503;-0.71226;-0.06553;,
 0.00000;-0.77337;-0.06553;,
 0.00000;-0.75499;-0.03535;,
 -0.00000;-0.74821;0.00000;,
 0.30570;-0.73802;-0.08535;,
 0.31820;-0.76820;-0.09268;,
 0.00000;-0.83410;-0.09268;,
 0.00000;-0.80133;-0.08535;,
 0.33070;-0.79838;-0.08535;,
 0.34137;-0.82413;-0.06553;,
 0.00000;-0.89483;-0.06553;,
 0.00000;-0.86687;-0.08535;,
 0.34838;-0.84105;-0.03535;,
 0.35096;-0.84730;-0.00000;,
 0.00000;-0.91999;0.00000;,
 0.00000;-0.91321;-0.03535;,
 0.34838;-0.84105;0.03535;,
 0.34137;-0.82413;0.06553;,
 -0.00000;-0.89483;0.06553;,
 -0.00000;-0.91321;0.03535;,
 0.33070;-0.79838;0.08535;,
 0.31820;-0.76820;0.09268;,
 -0.00000;-0.83410;0.09268;,
 -0.00000;-0.86687;0.08535;,
 0.30570;-0.73802;0.08535;,
 0.29503;-0.71226;0.06553;,
 -0.00000;-0.77337;0.06553;,
 -0.00000;-0.80133;0.08535;,
 0.28802;-0.69534;0.03535;,
 0.28543;-0.68909;0.00000;,
 -0.00000;-0.74821;0.00000;,
 -0.00000;-0.75499;0.03535;,
 -0.28802;-0.69534;-0.03535;,
 -0.28543;-0.68909;0.00000;,
 -0.29503;-0.71226;-0.06553;,
 -0.54685;-0.54685;-0.06553;,
 -0.53386;-0.53386;-0.03535;,
 -0.52906;-0.52906;0.00000;,
 -0.30570;-0.73802;-0.08535;,
 -0.31820;-0.76820;-0.09268;,
 -0.58980;-0.58980;-0.09268;,
 -0.56663;-0.56663;-0.08535;,
 -0.33070;-0.79838;-0.08535;,
 -0.34137;-0.82413;-0.06553;,
 -0.63274;-0.63274;-0.06553;,
 -0.61297;-0.61297;-0.08535;,
 -0.34838;-0.84105;-0.03535;,
 -0.35096;-0.84730;0.00000;,
 -0.65053;-0.65053;0.00000;,
 -0.64573;-0.64573;-0.03535;,
 -0.34838;-0.84105;0.03535;,
 -0.34137;-0.82413;0.06553;,
 -0.63274;-0.63274;0.06553;,
 -0.64573;-0.64573;0.03535;,
 -0.33070;-0.79838;0.08535;,
 -0.31820;-0.76820;0.09268;,
 -0.58980;-0.58980;0.09268;,
 -0.61297;-0.61297;0.08535;,
 -0.30570;-0.73802;0.08535;,
 -0.29503;-0.71226;0.06553;,
 -0.54685;-0.54685;0.06553;,
 -0.56663;-0.56663;0.08535;,
 -0.28802;-0.69534;0.03535;,
 -0.28543;-0.68909;0.00000;,
 -0.52906;-0.52906;0.00000;,
 -0.53386;-0.53386;0.03535;,
 -0.69534;-0.28802;-0.03535;,
 -0.68909;-0.28543;0.00000;,
 -0.71226;-0.29503;-0.06553;,
 -0.77337;0.00000;-0.06553;,
 -0.75499;0.00000;-0.03535;,
 -0.74821;0.00000;0.00000;,
 -0.73802;-0.30570;-0.08535;,
 -0.76820;-0.31820;-0.09268;,
 -0.83410;0.00000;-0.09268;,
 -0.80133;0.00000;-0.08535;,
 -0.79838;-0.33070;-0.08535;,
 -0.82413;-0.34137;-0.06553;,
 -0.89483;0.00000;-0.06553;,
 -0.86687;0.00000;-0.08535;,
 -0.84105;-0.34838;-0.03535;,
 -0.84730;-0.35096;0.00000;,
 -0.91999;0.00000;0.00000;,
 -0.91320;0.00000;-0.03535;,
 -0.84105;-0.34838;0.03535;,
 -0.82413;-0.34137;0.06553;,
 -0.89483;0.00000;0.06553;,
 -0.91320;0.00000;0.03535;,
 -0.79838;-0.33070;0.08535;,
 -0.76820;-0.31820;0.09268;,
 -0.83410;0.00000;0.09268;,
 -0.86687;0.00000;0.08535;,
 -0.73802;-0.30570;0.08535;,
 -0.71226;-0.29503;0.06553;,
 -0.77337;0.00000;0.06553;,
 -0.80133;0.00000;0.08535;,
 -0.69534;-0.28802;0.03535;,
 -0.68909;-0.28543;0.00000;,
 -0.74821;0.00000;0.00000;,
 -0.75499;0.00000;0.03535;,
 -0.69534;0.28802;-0.03535;,
 -0.68909;0.28543;0.00000;,
 -0.71226;0.29503;-0.06553;,
 -0.54685;0.54685;-0.06553;,
 -0.53386;0.53386;-0.03535;,
 -0.52906;0.52906;0.00000;,
 -0.73802;0.30570;-0.08535;,
 -0.76820;0.31820;-0.09268;,
 -0.58980;0.58980;-0.09268;,
 -0.56663;0.56663;-0.08535;,
 -0.79838;0.33070;-0.08535;,
 -0.82413;0.34137;-0.06553;,
 -0.63274;0.63274;-0.06553;,
 -0.61297;0.61297;-0.08535;,
 -0.84105;0.34838;-0.03535;,
 -0.84730;0.35096;0.00000;,
 -0.65053;0.65053;0.00000;,
 -0.64573;0.64573;-0.03535;,
 -0.84105;0.34838;0.03535;,
 -0.82413;0.34137;0.06553;,
 -0.63274;0.63274;0.06553;,
 -0.64573;0.64573;0.03535;,
 -0.79838;0.33070;0.08535;,
 -0.76820;0.31820;0.09268;,
 -0.58980;0.58980;0.09268;,
 -0.61297;0.61297;0.08535;,
 -0.73802;0.30570;0.08535;,
 -0.71226;0.29503;0.06553;,
 -0.54685;0.54685;0.06553;,
 -0.56663;0.56663;0.08535;,
 -0.69534;0.28802;0.03535;,
 -0.68909;0.28543;0.00000;,
 -0.52906;0.52906;0.00000;,
 -0.53386;0.53386;0.03535;,
 -0.28802;0.69534;-0.03535;,
 -0.28543;0.68909;0.00000;,
 -0.29503;0.71226;-0.06553;,
 0.00000;0.77337;-0.06553;,
 0.00000;0.75499;-0.03535;,
 0.00000;0.74821;0.00000;,
 -0.30570;0.73802;-0.08535;,
 -0.31820;0.76820;-0.09268;,
 0.00000;0.83410;-0.09268;,
 0.00000;0.80133;-0.08535;,
 -0.33070;0.79838;-0.08535;,
 -0.34137;0.82413;-0.06553;,
 0.00000;0.89483;-0.06553;,
 0.00000;0.86687;-0.08535;,
 -0.34838;0.84105;-0.03535;,
 -0.35096;0.84730;0.00000;,
 0.00000;0.91999;0.00000;,
 0.00000;0.91321;-0.03535;,
 -0.34838;0.84105;0.03535;,
 -0.34137;0.82413;0.06553;,
 0.00000;0.89483;0.06553;,
 0.00000;0.91321;0.03535;,
 -0.33070;0.79838;0.08535;,
 -0.31820;0.76820;0.09268;,
 0.00000;0.83410;0.09268;,
 0.00000;0.86687;0.08535;,
 -0.30570;0.73802;0.08535;,
 -0.29503;0.71226;0.06553;,
 0.00000;0.77337;0.06553;,
 0.00000;0.80133;0.08535;,
 -0.28802;0.69534;0.03535;,
 -0.28543;0.68909;0.00000;,
 0.00000;0.74821;0.00000;,
 0.00000;0.75499;0.03535;,
 0.28802;0.69534;-0.03535;,
 0.28543;0.68909;0.00000;,
 0.29503;0.71226;-0.06553;,
 0.54685;0.54685;-0.06553;,
 0.53386;0.53386;-0.03535;,
 0.52906;0.52906;0.00000;,
 0.30570;0.73802;-0.08535;,
 0.31820;0.76820;-0.09268;,
 0.58980;0.58980;-0.09268;,
 0.56663;0.56663;-0.08535;,
 0.33070;0.79838;-0.08535;,
 0.34137;0.82413;-0.06553;,
 0.63274;0.63274;-0.06553;,
 0.61297;0.61297;-0.08535;,
 0.34838;0.84105;-0.03535;,
 0.35096;0.84730;0.00000;,
 0.65053;0.65053;0.00000;,
 0.64573;0.64573;-0.03535;,
 0.34838;0.84105;0.03535;,
 0.34137;0.82413;0.06553;,
 0.63274;0.63274;0.06553;,
 0.64573;0.64573;0.03535;,
 0.33070;0.79838;0.08535;,
 0.31820;0.76820;0.09268;,
 0.58980;0.58980;0.09268;,
 0.61297;0.61297;0.08535;,
 0.30570;0.73802;0.08535;,
 0.29503;0.71226;0.06553;,
 0.54685;0.54685;0.06553;,
 0.56663;0.56663;0.08535;,
 0.28802;0.69534;0.03535;,
 0.28543;0.68909;0.00000;,
 0.52906;0.52906;0.00000;,
 0.53386;0.53386;0.03535;,
 0.69534;0.28802;-0.03535;,
 0.68909;0.28543;0.00000;,
 0.71226;0.29503;-0.06553;,
 0.77337;0.00000;-0.06553;,
 0.75499;0.00000;-0.03535;,
 0.74821;0.00000;-0.00000;,
 0.73802;0.30570;-0.08535;,
 0.76820;0.31820;-0.09268;,
 0.83410;0.00000;-0.09268;,
 0.80133;0.00000;-0.08535;,
 0.79838;0.33070;-0.08535;,
 0.82413;0.34137;-0.06553;,
 0.89483;0.00000;-0.06553;,
 0.86687;0.00000;-0.08535;,
 0.84105;0.34838;-0.03535;,
 0.84730;0.35096;0.00000;,
 0.91999;0.00000;0.00000;,
 0.91320;0.00000;-0.03535;,
 0.84105;0.34838;0.03535;,
 0.82413;0.34137;0.06553;,
 0.89483;0.00000;0.06553;,
 0.91320;0.00000;0.03535;,
 0.79838;0.33070;0.08535;,
 0.76820;0.31820;0.09268;,
 0.83410;0.00000;0.09268;,
 0.86687;0.00000;0.08535;,
 0.73802;0.30570;0.08535;,
 0.71226;0.29503;0.06553;,
 0.77337;0.00000;0.06553;,
 0.80133;0.00000;0.08535;,
 0.69534;0.28802;0.03535;,
 0.68909;0.28543;0.00000;,
 0.74821;0.00000;-0.00000;,
 0.75499;0.00000;0.03535;;
 
 256;
 4;0,1,2,3;,
 4;4,5,2,1;,
 4;6,7,2,5;,
 4;8,3,2,7;,
 4;4,9,10,5;,
 4;11,12,10,9;,
 4;13,14,10,12;,
 4;6,5,10,14;,
 4;11,15,16,12;,
 4;17,18,16,15;,
 4;19,20,16,18;,
 4;13,12,16,20;,
 4;17,21,22,18;,
 4;23,24,22,21;,
 4;25,26,22,24;,
 4;19,18,22,26;,
 4;23,27,28,24;,
 4;29,30,28,27;,
 4;31,32,28,30;,
 4;25,24,28,32;,
 4;29,33,34,30;,
 4;35,36,34,33;,
 4;37,38,34,36;,
 4;31,30,34,38;,
 4;35,39,40,36;,
 4;41,42,40,39;,
 4;43,44,40,42;,
 4;37,36,40,44;,
 4;41,45,46,42;,
 4;47,48,46,45;,
 4;49,50,46,48;,
 4;43,42,46,50;,
 4;8,7,51,52;,
 4;6,53,51,7;,
 4;54,55,51,53;,
 4;56,52,51,55;,
 4;6,14,57,53;,
 4;13,58,57,14;,
 4;59,60,57,58;,
 4;54,53,57,60;,
 4;13,20,61,58;,
 4;19,62,61,20;,
 4;63,64,61,62;,
 4;59,58,61,64;,
 4;19,26,65,62;,
 4;25,66,65,26;,
 4;67,68,65,66;,
 4;63,62,65,68;,
 4;25,32,69,66;,
 4;31,70,69,32;,
 4;71,72,69,70;,
 4;67,66,69,72;,
 4;31,38,73,70;,
 4;37,74,73,38;,
 4;75,76,73,74;,
 4;71,70,73,76;,
 4;37,44,77,74;,
 4;43,78,77,44;,
 4;79,80,77,78;,
 4;75,74,77,80;,
 4;43,50,81,78;,
 4;49,82,81,50;,
 4;83,84,81,82;,
 4;79,78,81,84;,
 4;56,55,85,86;,
 4;54,87,85,55;,
 4;88,89,85,87;,
 4;90,86,85,89;,
 4;54,60,91,87;,
 4;59,92,91,60;,
 4;93,94,91,92;,
 4;88,87,91,94;,
 4;59,64,95,92;,
 4;63,96,95,64;,
 4;97,98,95,96;,
 4;93,92,95,98;,
 4;63,68,99,96;,
 4;67,100,99,68;,
 4;101,102,99,100;,
 4;97,96,99,102;,
 4;67,72,103,100;,
 4;71,104,103,72;,
 4;105,106,103,104;,
 4;101,100,103,106;,
 4;71,76,107,104;,
 4;75,108,107,76;,
 4;109,110,107,108;,
 4;105,104,107,110;,
 4;75,80,111,108;,
 4;79,112,111,80;,
 4;113,114,111,112;,
 4;109,108,111,114;,
 4;79,84,115,112;,
 4;83,116,115,84;,
 4;117,118,115,116;,
 4;113,112,115,118;,
 4;90,89,119,120;,
 4;88,121,119,89;,
 4;122,123,119,121;,
 4;124,120,119,123;,
 4;88,94,125,121;,
 4;93,126,125,94;,
 4;127,128,125,126;,
 4;122,121,125,128;,
 4;93,98,129,126;,
 4;97,130,129,98;,
 4;131,132,129,130;,
 4;127,126,129,132;,
 4;97,102,133,130;,
 4;101,134,133,102;,
 4;135,136,133,134;,
 4;131,130,133,136;,
 4;101,106,137,134;,
 4;105,138,137,106;,
 4;139,140,137,138;,
 4;135,134,137,140;,
 4;105,110,141,138;,
 4;109,142,141,110;,
 4;143,144,141,142;,
 4;139,138,141,144;,
 4;109,114,145,142;,
 4;113,146,145,114;,
 4;147,148,145,146;,
 4;143,142,145,148;,
 4;113,118,149,146;,
 4;117,150,149,118;,
 4;151,152,149,150;,
 4;147,146,149,152;,
 4;124,123,153,154;,
 4;122,155,153,123;,
 4;156,157,153,155;,
 4;158,154,153,157;,
 4;122,128,159,155;,
 4;127,160,159,128;,
 4;161,162,159,160;,
 4;156,155,159,162;,
 4;127,132,163,160;,
 4;131,164,163,132;,
 4;165,166,163,164;,
 4;161,160,163,166;,
 4;131,136,167,164;,
 4;135,168,167,136;,
 4;169,170,167,168;,
 4;165,164,167,170;,
 4;135,140,171,168;,
 4;139,172,171,140;,
 4;173,174,171,172;,
 4;169,168,171,174;,
 4;139,144,175,172;,
 4;143,176,175,144;,
 4;177,178,175,176;,
 4;173,172,175,178;,
 4;143,148,179,176;,
 4;147,180,179,148;,
 4;181,182,179,180;,
 4;177,176,179,182;,
 4;147,152,183,180;,
 4;151,184,183,152;,
 4;185,186,183,184;,
 4;181,180,183,186;,
 4;158,157,187,188;,
 4;156,189,187,157;,
 4;190,191,187,189;,
 4;192,188,187,191;,
 4;156,162,193,189;,
 4;161,194,193,162;,
 4;195,196,193,194;,
 4;190,189,193,196;,
 4;161,166,197,194;,
 4;165,198,197,166;,
 4;199,200,197,198;,
 4;195,194,197,200;,
 4;165,170,201,198;,
 4;169,202,201,170;,
 4;203,204,201,202;,
 4;199,198,201,204;,
 4;169,174,205,202;,
 4;173,206,205,174;,
 4;207,208,205,206;,
 4;203,202,205,208;,
 4;173,178,209,206;,
 4;177,210,209,178;,
 4;211,212,209,210;,
 4;207,206,209,212;,
 4;177,182,213,210;,
 4;181,214,213,182;,
 4;215,216,213,214;,
 4;211,210,213,216;,
 4;181,186,217,214;,
 4;185,218,217,186;,
 4;219,220,217,218;,
 4;215,214,217,220;,
 4;192,191,221,222;,
 4;190,223,221,191;,
 4;224,225,221,223;,
 4;226,222,221,225;,
 4;190,196,227,223;,
 4;195,228,227,196;,
 4;229,230,227,228;,
 4;224,223,227,230;,
 4;195,200,231,228;,
 4;199,232,231,200;,
 4;233,234,231,232;,
 4;229,228,231,234;,
 4;199,204,235,232;,
 4;203,236,235,204;,
 4;237,238,235,236;,
 4;233,232,235,238;,
 4;203,208,239,236;,
 4;207,240,239,208;,
 4;241,242,239,240;,
 4;237,236,239,242;,
 4;207,212,243,240;,
 4;211,244,243,212;,
 4;245,246,243,244;,
 4;241,240,243,246;,
 4;211,216,247,244;,
 4;215,248,247,216;,
 4;249,250,247,248;,
 4;245,244,247,250;,
 4;215,220,251,248;,
 4;219,252,251,220;,
 4;253,254,251,252;,
 4;249,248,251,254;,
 4;226,225,255,256;,
 4;224,257,255,225;,
 4;258,259,255,257;,
 4;260,256,255,259;,
 4;224,230,261,257;,
 4;229,262,261,230;,
 4;263,264,261,262;,
 4;258,257,261,264;,
 4;229,234,265,262;,
 4;233,266,265,234;,
 4;267,268,265,266;,
 4;263,262,265,268;,
 4;233,238,269,266;,
 4;237,270,269,238;,
 4;271,272,269,270;,
 4;267,266,269,272;,
 4;237,242,273,270;,
 4;241,274,273,242;,
 4;275,276,273,274;,
 4;271,270,273,276;,
 4;241,246,277,274;,
 4;245,278,277,246;,
 4;279,280,277,278;,
 4;275,274,277,280;,
 4;245,250,281,278;,
 4;249,282,281,250;,
 4;283,284,281,282;,
 4;279,278,281,284;,
 4;249,254,285,282;,
 4;253,286,285,254;,
 4;287,288,285,286;,
 4;283,282,285,288;;
 
 MeshMaterialList {
  1;
  256;
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
  0;;
  Material {
   0.000000;0.627451;0.800000;0.800000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.588235;0.750000;;
  }
 }
 MeshNormals {
  254;
  -0.862152;0.357115;-0.359392;,
  -0.375685;0.155614;-0.913589;,
  0.375684;-0.155613;-0.913590;,
  0.862151;-0.357114;-0.359395;,
  0.862151;-0.357114;0.359395;,
  0.375683;-0.155613;0.913590;,
  -0.375686;0.155614;0.913589;,
  -0.862152;0.357115;0.359392;,
  -0.357114;0.862150;-0.359398;,
  -0.155614;0.375685;-0.913589;,
  0.155614;-0.375684;-0.913590;,
  0.357115;-0.862150;-0.359397;,
  0.357115;-0.862150;0.359397;,
  0.155613;-0.375684;0.913590;,
  -0.155614;0.375685;0.913589;,
  -0.357114;0.862150;0.359398;,
  0.357114;0.862150;-0.359398;,
  0.155614;0.375685;-0.913589;,
  -0.155614;-0.375684;-0.913590;,
  -0.357115;-0.862150;-0.359397;,
  -0.357115;-0.862150;0.359397;,
  -0.155613;-0.375684;0.913590;,
  0.155614;0.375685;0.913589;,
  0.357114;0.862150;0.359398;,
  0.862152;0.357115;-0.359392;,
  0.375685;0.155614;-0.913589;,
  -0.375684;-0.155613;-0.913590;,
  -0.862151;-0.357114;-0.359395;,
  -0.862151;-0.357114;0.359395;,
  -0.375683;-0.155613;0.913590;,
  0.375686;0.155614;0.913589;,
  0.862152;0.357115;0.359392;,
  0.862151;-0.357115;-0.359395;,
  0.375685;-0.155614;-0.913589;,
  -0.375684;0.155613;-0.913590;,
  -0.862152;0.357115;-0.359393;,
  -0.862152;0.357115;0.359393;,
  -0.375684;0.155613;0.913590;,
  0.375685;-0.155614;0.913589;,
  0.862151;-0.357115;0.359395;,
  0.357113;-0.862149;-0.359402;,
  0.155614;-0.375685;-0.913589;,
  -0.155614;0.375684;-0.913590;,
  -0.357115;0.862152;-0.359393;,
  -0.357115;0.862152;0.359393;,
  -0.155614;0.375684;0.913589;,
  0.155614;-0.375685;0.913589;,
  0.357113;-0.862149;0.359402;,
  -0.357114;-0.862150;-0.359398;,
  -0.155614;-0.375685;-0.913589;,
  0.155614;0.375684;-0.913590;,
  0.357115;0.862151;-0.359395;,
  0.357115;0.862151;0.359395;,
  0.155614;0.375684;0.913590;,
  -0.155614;-0.375685;0.913589;,
  -0.357114;-0.862150;0.359398;,
  -0.862152;-0.357115;-0.359392;,
  -0.375685;-0.155614;-0.913589;,
  0.375684;0.155613;-0.913590;,
  0.862152;0.357115;-0.359393;,
  0.862152;0.357115;0.359393;,
  0.375684;0.155613;0.913590;,
  -0.375685;-0.155614;0.913589;,
  -0.862152;-0.357115;0.359392;,
  -0.932808;0.000000;-0.360374;,
  -0.676106;0.280053;-0.681507;,
  -0.659595;0.659595;-0.360375;,
  -0.923880;0.382684;-0.000000;,
  -0.405578;0.000000;-0.914061;,
  -0.000001;-0.000000;-1.000000;,
  -0.286786;0.286786;-0.914061;,
  0.405576;-0.000000;-0.914062;,
  0.676107;-0.280053;-0.681506;,
  0.286785;-0.286785;-0.914061;,
  0.932808;0.000000;-0.360375;,
  0.923880;-0.382683;0.000000;,
  0.659594;-0.659593;-0.360379;,
  0.932808;0.000000;0.360375;,
  0.676107;-0.280053;0.681506;,
  0.659594;-0.659593;0.360379;,
  0.405576;-0.000000;0.914062;,
  -0.000001;0.000000;1.000000;,
  0.286785;-0.286785;0.914062;,
  -0.405578;0.000000;0.914061;,
  -0.676106;0.280053;0.681506;,
  -0.286787;0.286786;0.914061;,
  -0.932808;0.000000;0.360374;,
  -0.659595;0.659595;0.360375;,
  -0.280050;0.676102;-0.681512;,
  0.000000;0.932806;-0.360380;,
  -0.382683;0.923880;0.000000;,
  0.000001;-0.000003;-1.000000;,
  0.000000;0.405576;-0.914061;,
  0.280051;-0.676102;-0.681511;,
  -0.000000;-0.405576;-0.914061;,
  0.382684;-0.923879;-0.000000;,
  -0.000000;-0.932807;-0.360375;,
  0.280051;-0.676102;0.681511;,
  -0.000000;-0.932807;0.360375;,
  0.000000;-0.000002;1.000000;,
  -0.000000;-0.405576;0.914061;,
  -0.280050;0.676102;0.681512;,
  0.000000;0.405576;0.914061;,
  0.000000;0.932806;0.360380;,
  0.280050;0.676102;-0.681512;,
  0.659595;0.659595;-0.360375;,
  0.382683;0.923880;0.000000;,
  -0.000001;-0.000003;-1.000000;,
  0.286786;0.286786;-0.914061;,
  -0.280051;-0.676102;-0.681511;,
  -0.286785;-0.286785;-0.914061;,
  -0.382684;-0.923879;0.000000;,
  -0.659594;-0.659593;-0.360379;,
  -0.280051;-0.676102;0.681511;,
  -0.659594;-0.659593;0.360379;,
  -0.000000;-0.000002;1.000000;,
  -0.286785;-0.286785;0.914062;,
  0.280050;0.676102;0.681512;,
  0.286787;0.286786;0.914061;,
  0.659595;0.659595;0.360375;,
  0.676106;0.280053;-0.681507;,
  0.932808;0.000000;-0.360374;,
  0.923879;0.382683;-0.000000;,
  0.000001;-0.000000;-1.000000;,
  0.405578;0.000000;-0.914061;,
  -0.676107;-0.280053;-0.681506;,
  -0.405576;-0.000000;-0.914062;,
  -0.923880;-0.382683;0.000000;,
  -0.932808;0.000000;-0.360375;,
  -0.676107;-0.280053;0.681506;,
  -0.932808;0.000000;0.360375;,
  0.000001;0.000000;1.000000;,
  -0.405576;-0.000000;0.914062;,
  0.676106;0.280053;0.681506;,
  0.405578;0.000000;0.914061;,
  0.932808;0.000000;0.360374;,
  0.676106;-0.280053;-0.681507;,
  0.659593;-0.659593;-0.360381;,
  0.923879;-0.382684;-0.000000;,
  0.000001;0.000000;-1.000000;,
  0.286786;-0.286786;-0.914061;,
  -0.676107;0.280053;-0.681506;,
  -0.286785;0.286785;-0.914061;,
  -0.923879;0.382684;0.000000;,
  -0.659596;0.659595;-0.360373;,
  -0.676107;0.280053;0.681506;,
  -0.659596;0.659595;0.360373;,
  0.000001;0.000000;1.000000;,
  -0.286785;0.286785;0.914061;,
  0.676106;-0.280053;0.681506;,
  0.286786;-0.286786;0.914061;,
  0.659593;-0.659593;0.360381;,
  0.280050;-0.676102;-0.681512;,
  0.000000;-0.932805;-0.360380;,
  0.382683;-0.923880;-0.000000;,
  -0.000001;0.000003;-1.000000;,
  -0.000000;-0.405576;-0.914061;,
  -0.280051;0.676102;-0.681511;,
  0.000000;0.405576;-0.914061;,
  -0.382684;0.923879;0.000000;,
  -0.000000;0.932808;-0.360375;,
  -0.280051;0.676102;0.681511;,
  -0.000000;0.932808;0.360375;,
  -0.000001;0.000003;1.000000;,
  0.000000;0.405576;0.914061;,
  0.280050;-0.676102;0.681512;,
  -0.000000;-0.405576;0.914061;,
  0.000000;-0.932806;0.360380;,
  -0.280050;-0.676102;-0.681512;,
  -0.659595;-0.659595;-0.360375;,
  -0.382683;-0.923880;0.000000;,
  0.000001;0.000003;-1.000000;,
  -0.286786;-0.286786;-0.914061;,
  0.280051;0.676102;-0.681511;,
  0.286785;0.286785;-0.914061;,
  0.382684;0.923879;0.000000;,
  0.659595;0.659594;-0.360374;,
  0.280051;0.676102;0.681511;,
  0.659595;0.659594;0.360374;,
  0.000001;0.000003;1.000000;,
  0.286785;0.286785;0.914061;,
  -0.280050;-0.676102;0.681512;,
  -0.286786;-0.286786;0.914061;,
  -0.659595;-0.659595;0.360375;,
  -0.676106;-0.280053;-0.681507;,
  -0.923879;-0.382683;-0.000000;,
  -0.000001;0.000000;-1.000000;,
  0.676107;0.280053;-0.681506;,
  0.923879;0.382684;0.000000;,
  0.676107;0.280053;0.681506;,
  -0.000001;0.000000;1.000000;,
  -0.676106;-0.280053;0.681506;,
  -1.000000;0.000000;0.000000;,
  -0.730746;0.000000;-0.682649;,
  -0.000002;0.000000;-1.000000;,
  0.730750;0.000000;-0.682645;,
  1.000000;0.000000;0.000000;,
  0.730750;0.000000;0.682645;,
  -0.000002;0.000000;1.000000;,
  -0.730746;0.000000;0.682649;,
  -0.707106;0.707107;0.000000;,
  -0.516715;0.516714;-0.682651;,
  0.000001;-0.000001;-1.000000;,
  0.516715;-0.516714;-0.682651;,
  0.707108;-0.707106;-0.000000;,
  0.516715;-0.516714;0.682651;,
  -0.000001;0.000000;1.000000;,
  -0.516715;0.516714;0.682651;,
  0.000000;1.000000;0.000000;,
  0.000000;0.730741;-0.682654;,
  0.000000;-0.000003;-1.000000;,
  0.000000;-0.730743;-0.682653;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.730743;0.682653;,
  0.000000;-0.000003;1.000000;,
  0.000000;0.730741;0.682654;,
  0.707106;0.707107;0.000000;,
  0.516715;0.516714;-0.682651;,
  -0.000001;-0.000001;-1.000000;,
  -0.516715;-0.516714;-0.682651;,
  -0.707108;-0.707106;0.000000;,
  -0.516715;-0.516714;0.682651;,
  0.000001;0.000000;1.000000;,
  0.516715;0.516714;0.682651;,
  0.730746;0.000000;-0.682649;,
  0.000002;0.000000;-1.000000;,
  -0.730750;0.000000;-0.682645;,
  -0.730750;0.000000;0.682645;,
  0.000002;0.000000;1.000000;,
  0.730746;0.000000;0.682649;,
  0.707106;-0.707108;-0.000000;,
  0.516715;-0.516714;-0.682651;,
  -0.000001;0.000001;-1.000000;,
  -0.516715;0.516714;-0.682651;,
  -0.707107;0.707106;0.000000;,
  -0.516715;0.516714;0.682651;,
  -0.000001;0.000001;1.000000;,
  0.516715;-0.516714;0.682651;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.730741;-0.682654;,
  0.000000;0.000003;-1.000000;,
  0.000000;0.730743;-0.682653;,
  -0.000000;1.000000;0.000000;,
  -0.000000;0.730743;0.682653;,
  0.000000;0.000003;1.000000;,
  0.000000;-0.730741;0.682654;,
  -0.707106;-0.707107;0.000000;,
  -0.516715;-0.516714;-0.682651;,
  0.000001;0.000001;-1.000000;,
  0.516715;0.516714;-0.682651;,
  0.707108;0.707106;0.000000;,
  0.516715;0.516714;0.682651;,
  0.000001;0.000001;1.000000;,
  -0.516715;-0.516714;0.682651;;
  256;
  4;192,64,0,67;,
  4;193,65,0,64;,
  4;201,66,0,65;,
  4;200,67,0,66;,
  4;193,68,1,65;,
  4;194,69,1,68;,
  4;202,70,1,69;,
  4;201,65,1,70;,
  4;194,71,2,69;,
  4;195,72,2,71;,
  4;203,73,2,72;,
  4;202,69,2,73;,
  4;195,74,3,72;,
  4;196,75,3,74;,
  4;204,76,3,75;,
  4;203,72,3,76;,
  4;196,77,4,75;,
  4;197,78,4,77;,
  4;205,79,4,78;,
  4;204,75,4,79;,
  4;197,80,5,78;,
  4;198,81,5,80;,
  4;206,82,5,81;,
  4;205,78,5,82;,
  4;198,83,6,81;,
  4;199,84,6,83;,
  4;207,85,6,84;,
  4;206,81,6,85;,
  4;199,86,7,84;,
  4;192,67,7,86;,
  4;200,87,7,67;,
  4;207,84,7,87;,
  4;200,66,8,90;,
  4;201,88,8,66;,
  4;209,89,8,88;,
  4;208,90,8,89;,
  4;201,70,9,88;,
  4;202,91,9,70;,
  4;210,92,9,91;,
  4;209,88,9,92;,
  4;202,73,10,91;,
  4;203,93,10,73;,
  4;211,94,10,93;,
  4;210,91,10,94;,
  4;203,76,11,93;,
  4;204,95,11,76;,
  4;212,96,11,95;,
  4;211,93,11,96;,
  4;204,79,12,95;,
  4;205,97,12,79;,
  4;213,98,12,97;,
  4;212,95,12,98;,
  4;205,82,13,97;,
  4;206,99,13,82;,
  4;214,100,13,99;,
  4;213,97,13,100;,
  4;206,85,14,99;,
  4;207,101,14,85;,
  4;215,102,14,101;,
  4;214,99,14,102;,
  4;207,87,15,101;,
  4;200,90,15,87;,
  4;208,103,15,90;,
  4;215,101,15,103;,
  4;208,89,16,106;,
  4;209,104,16,89;,
  4;217,105,16,104;,
  4;216,106,16,105;,
  4;209,92,17,104;,
  4;210,107,17,92;,
  4;218,108,17,107;,
  4;217,104,17,108;,
  4;210,94,18,107;,
  4;211,109,18,94;,
  4;219,110,18,109;,
  4;218,107,18,110;,
  4;211,96,19,109;,
  4;212,111,19,96;,
  4;220,112,19,111;,
  4;219,109,19,112;,
  4;212,98,20,111;,
  4;213,113,20,98;,
  4;221,114,20,113;,
  4;220,111,20,114;,
  4;213,100,21,113;,
  4;214,115,21,100;,
  4;222,116,21,115;,
  4;221,113,21,116;,
  4;214,102,22,115;,
  4;215,117,22,102;,
  4;223,118,22,117;,
  4;222,115,22,118;,
  4;215,103,23,117;,
  4;208,106,23,103;,
  4;216,119,23,106;,
  4;223,117,23,119;,
  4;216,105,24,122;,
  4;217,120,24,105;,
  4;224,121,24,120;,
  4;196,122,24,121;,
  4;217,108,25,120;,
  4;218,123,25,108;,
  4;225,124,25,123;,
  4;224,120,25,124;,
  4;218,110,26,123;,
  4;219,125,26,110;,
  4;226,126,26,125;,
  4;225,123,26,126;,
  4;219,112,27,125;,
  4;220,127,27,112;,
  4;192,128,27,127;,
  4;226,125,27,128;,
  4;220,114,28,127;,
  4;221,129,28,114;,
  4;227,130,28,129;,
  4;192,127,28,130;,
  4;221,116,29,129;,
  4;222,131,29,116;,
  4;228,132,29,131;,
  4;227,129,29,132;,
  4;222,118,30,131;,
  4;223,133,30,118;,
  4;229,134,30,133;,
  4;228,131,30,134;,
  4;223,119,31,133;,
  4;216,122,31,119;,
  4;196,135,31,122;,
  4;229,133,31,135;,
  4;196,121,32,138;,
  4;224,136,32,121;,
  4;231,137,32,136;,
  4;230,138,32,137;,
  4;224,124,33,136;,
  4;225,139,33,124;,
  4;232,140,33,139;,
  4;231,136,33,140;,
  4;225,126,34,139;,
  4;226,141,34,126;,
  4;233,142,34,141;,
  4;232,139,34,142;,
  4;226,128,35,141;,
  4;192,143,35,128;,
  4;234,144,35,143;,
  4;233,141,35,144;,
  4;192,130,36,143;,
  4;227,145,36,130;,
  4;235,146,36,145;,
  4;234,143,36,146;,
  4;227,132,37,145;,
  4;228,147,37,132;,
  4;236,148,37,147;,
  4;235,145,37,148;,
  4;228,134,38,147;,
  4;229,149,38,134;,
  4;237,150,38,149;,
  4;236,147,38,150;,
  4;229,135,39,149;,
  4;196,138,39,135;,
  4;230,151,39,138;,
  4;237,149,39,151;,
  4;230,137,40,154;,
  4;231,152,40,137;,
  4;239,153,40,152;,
  4;238,154,40,153;,
  4;231,140,41,152;,
  4;232,155,41,140;,
  4;240,156,41,155;,
  4;239,152,41,156;,
  4;232,142,42,155;,
  4;233,157,42,142;,
  4;241,158,42,157;,
  4;240,155,42,158;,
  4;233,144,43,157;,
  4;234,159,43,144;,
  4;242,160,43,159;,
  4;241,157,43,160;,
  4;234,146,44,159;,
  4;235,161,44,146;,
  4;243,162,44,161;,
  4;242,159,44,162;,
  4;235,148,45,161;,
  4;236,163,45,148;,
  4;244,164,45,163;,
  4;243,161,45,164;,
  4;236,150,46,163;,
  4;237,165,46,150;,
  4;245,166,46,165;,
  4;244,163,46,166;,
  4;237,151,47,165;,
  4;230,154,47,151;,
  4;238,167,47,154;,
  4;245,165,47,167;,
  4;238,153,48,170;,
  4;239,168,48,153;,
  4;247,169,48,168;,
  4;246,170,48,169;,
  4;239,156,49,168;,
  4;240,171,49,156;,
  4;248,172,49,171;,
  4;247,168,49,172;,
  4;240,158,50,171;,
  4;241,173,50,158;,
  4;249,174,50,173;,
  4;248,171,50,174;,
  4;241,160,51,173;,
  4;242,175,51,160;,
  4;250,176,51,175;,
  4;249,173,51,176;,
  4;242,162,52,175;,
  4;243,177,52,162;,
  4;251,178,52,177;,
  4;250,175,52,178;,
  4;243,164,53,177;,
  4;244,179,53,164;,
  4;252,180,53,179;,
  4;251,177,53,180;,
  4;244,166,54,179;,
  4;245,181,54,166;,
  4;253,182,54,181;,
  4;252,179,54,182;,
  4;245,167,55,181;,
  4;238,170,55,167;,
  4;246,183,55,170;,
  4;253,181,55,183;,
  4;246,169,56,185;,
  4;247,184,56,169;,
  4;193,64,56,184;,
  4;192,185,56,64;,
  4;247,172,57,184;,
  4;248,186,57,172;,
  4;194,68,57,186;,
  4;193,184,57,68;,
  4;248,174,58,186;,
  4;249,187,58,174;,
  4;195,71,58,187;,
  4;194,186,58,71;,
  4;249,176,59,187;,
  4;250,188,59,176;,
  4;196,74,59,188;,
  4;195,187,59,74;,
  4;250,178,60,188;,
  4;251,189,60,178;,
  4;197,77,60,189;,
  4;196,188,60,77;,
  4;251,180,61,189;,
  4;252,190,61,180;,
  4;198,80,61,190;,
  4;197,189,61,80;,
  4;252,182,62,190;,
  4;253,191,62,182;,
  4;199,83,62,191;,
  4;198,190,62,83;,
  4;253,183,63,191;,
  4;246,185,63,183;,
  4;192,86,63,185;,
  4;199,191,63,86;;
 }
 MeshTextureCoords {
  289;
  0.000000;0.000000;
  0.000000;0.062500;
  0.062500;0.062500;
  0.062500;0.000000;
  0.000000;0.125000;
  0.062500;0.125000;
  0.125000;0.125000;
  0.125000;0.062500;
  0.125000;0.000000;
  0.000000;0.187500;
  0.062500;0.187500;
  0.000000;0.250000;
  0.062500;0.250000;
  0.125000;0.250000;
  0.125000;0.187500;
  0.000000;0.312500;
  0.062500;0.312500;
  0.000000;0.375000;
  0.062500;0.375000;
  0.125000;0.375000;
  0.125000;0.312500;
  0.000000;0.437500;
  0.062500;0.437500;
  0.000000;0.500000;
  0.062500;0.500000;
  0.125000;0.500000;
  0.125000;0.437500;
  0.000000;0.562500;
  0.062500;0.562500;
  0.000000;0.625000;
  0.062500;0.625000;
  0.125000;0.625000;
  0.125000;0.562500;
  0.000000;0.687500;
  0.062500;0.687500;
  0.000000;0.750000;
  0.062500;0.750000;
  0.125000;0.750000;
  0.125000;0.687500;
  0.000000;0.812500;
  0.062500;0.812500;
  0.000000;0.875000;
  0.062500;0.875000;
  0.125000;0.875000;
  0.125000;0.812500;
  0.000000;0.937500;
  0.062500;0.937500;
  0.000000;1.000000;
  0.062500;1.000000;
  0.125000;1.000000;
  0.125000;0.937500;
  0.187500;0.062500;
  0.187500;0.000000;
  0.187500;0.125000;
  0.250000;0.125000;
  0.250000;0.062500;
  0.250000;0.000000;
  0.187500;0.187500;
  0.187500;0.250000;
  0.250000;0.250000;
  0.250000;0.187500;
  0.187500;0.312500;
  0.187500;0.375000;
  0.250000;0.375000;
  0.250000;0.312500;
  0.187500;0.437500;
  0.187500;0.500000;
  0.250000;0.500000;
  0.250000;0.437500;
  0.187500;0.562500;
  0.187500;0.625000;
  0.250000;0.625000;
  0.250000;0.562500;
  0.187500;0.687500;
  0.187500;0.750000;
  0.250000;0.750000;
  0.250000;0.687500;
  0.187500;0.812500;
  0.187500;0.875000;
  0.250000;0.875000;
  0.250000;0.812500;
  0.187500;0.937500;
  0.187500;1.000000;
  0.250000;1.000000;
  0.250000;0.937500;
  0.312500;0.062500;
  0.312500;0.000000;
  0.312500;0.125000;
  0.375000;0.125000;
  0.375000;0.062500;
  0.375000;0.000000;
  0.312500;0.187500;
  0.312500;0.250000;
  0.375000;0.250000;
  0.375000;0.187500;
  0.312500;0.312500;
  0.312500;0.375000;
  0.375000;0.375000;
  0.375000;0.312500;
  0.312500;0.437500;
  0.312500;0.500000;
  0.375000;0.500000;
  0.375000;0.437500;
  0.312500;0.562500;
  0.312500;0.625000;
  0.375000;0.625000;
  0.375000;0.562500;
  0.312500;0.687500;
  0.312500;0.750000;
  0.375000;0.750000;
  0.375000;0.687500;
  0.312500;0.812500;
  0.312500;0.875000;
  0.375000;0.875000;
  0.375000;0.812500;
  0.312500;0.937500;
  0.312500;1.000000;
  0.375000;1.000000;
  0.375000;0.937500;
  0.437500;0.062500;
  0.437500;0.000000;
  0.437500;0.125000;
  0.500000;0.125000;
  0.500000;0.062500;
  0.500000;0.000000;
  0.437500;0.187500;
  0.437500;0.250000;
  0.500000;0.250000;
  0.500000;0.187500;
  0.437500;0.312500;
  0.437500;0.375000;
  0.500000;0.375000;
  0.500000;0.312500;
  0.437500;0.437500;
  0.437500;0.500000;
  0.500000;0.500000;
  0.500000;0.437500;
  0.437500;0.562500;
  0.437500;0.625000;
  0.500000;0.625000;
  0.500000;0.562500;
  0.437500;0.687500;
  0.437500;0.750000;
  0.500000;0.750000;
  0.500000;0.687500;
  0.437500;0.812500;
  0.437500;0.875000;
  0.500000;0.875000;
  0.500000;0.812500;
  0.437500;0.937500;
  0.437500;1.000000;
  0.500000;1.000000;
  0.500000;0.937500;
  0.562500;0.062500;
  0.562500;0.000000;
  0.562500;0.125000;
  0.625000;0.125000;
  0.625000;0.062500;
  0.625000;0.000000;
  0.562500;0.187500;
  0.562500;0.250000;
  0.625000;0.250000;
  0.625000;0.187500;
  0.562500;0.312500;
  0.562500;0.375000;
  0.625000;0.375000;
  0.625000;0.312500;
  0.562500;0.437500;
  0.562500;0.500000;
  0.625000;0.500000;
  0.625000;0.437500;
  0.562500;0.562500;
  0.562500;0.625000;
  0.625000;0.625000;
  0.625000;0.562500;
  0.562500;0.687500;
  0.562500;0.750000;
  0.625000;0.750000;
  0.625000;0.687500;
  0.562500;0.812500;
  0.562500;0.875000;
  0.625000;0.875000;
  0.625000;0.812500;
  0.562500;0.937500;
  0.562500;1.000000;
  0.625000;1.000000;
  0.625000;0.937500;
  0.687500;0.062500;
  0.687500;0.000000;
  0.687500;0.125000;
  0.750000;0.125000;
  0.750000;0.062500;
  0.750000;0.000000;
  0.687500;0.187500;
  0.687500;0.250000;
  0.750000;0.250000;
  0.750000;0.187500;
  0.687500;0.312500;
  0.687500;0.375000;
  0.750000;0.375000;
  0.750000;0.312500;
  0.687500;0.437500;
  0.687500;0.500000;
  0.750000;0.500000;
  0.750000;0.437500;
  0.687500;0.562500;
  0.687500;0.625000;
  0.750000;0.625000;
  0.750000;0.562500;
  0.687500;0.687500;
  0.687500;0.750000;
  0.750000;0.750000;
  0.750000;0.687500;
  0.687500;0.812500;
  0.687500;0.875000;
  0.750000;0.875000;
  0.750000;0.812500;
  0.687500;0.937500;
  0.687500;1.000000;
  0.750000;1.000000;
  0.750000;0.937500;
  0.812500;0.062500;
  0.812500;0.000000;
  0.812500;0.125000;
  0.875000;0.125000;
  0.875000;0.062500;
  0.875000;0.000000;
  0.812500;0.187500;
  0.812500;0.250000;
  0.875000;0.250000;
  0.875000;0.187500;
  0.812500;0.312500;
  0.812500;0.375000;
  0.875000;0.375000;
  0.875000;0.312500;
  0.812500;0.437500;
  0.812500;0.500000;
  0.875000;0.500000;
  0.875000;0.437500;
  0.812500;0.562500;
  0.812500;0.625000;
  0.875000;0.625000;
  0.875000;0.562500;
  0.812500;0.687500;
  0.812500;0.750000;
  0.875000;0.750000;
  0.875000;0.687500;
  0.812500;0.812500;
  0.812500;0.875000;
  0.875000;0.875000;
  0.875000;0.812500;
  0.812500;0.937500;
  0.812500;1.000000;
  0.875000;1.000000;
  0.875000;0.937500;
  0.937500;0.062500;
  0.937500;0.000000;
  0.937500;0.125000;
  1.000000;0.125000;
  1.000000;0.062500;
  1.000000;0.000000;
  0.937500;0.187500;
  0.937500;0.250000;
  1.000000;0.250000;
  1.000000;0.187500;
  0.937500;0.312500;
  0.937500;0.375000;
  1.000000;0.375000;
  1.000000;0.312500;
  0.937500;0.437500;
  0.937500;0.500000;
  1.000000;0.500000;
  1.000000;0.437500;
  0.937500;0.562500;
  0.937500;0.625000;
  1.000000;0.625000;
  1.000000;0.562500;
  0.937500;0.687500;
  0.937500;0.750000;
  1.000000;0.750000;
  1.000000;0.687500;
  0.937500;0.812500;
  0.937500;0.875000;
  1.000000;0.875000;
  1.000000;0.812500;
  0.937500;0.937500;
  0.937500;1.000000;
  1.000000;1.000000;
  1.000000;0.937500;;
 }
}