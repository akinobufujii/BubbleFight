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
 24;
 -0.16385;-0.00974;-0.05323;,
 -0.07821;-0.00974;-0.15349;,
 -0.45400;-8.53812;-0.89101;,
 -0.95107;-8.53812;-0.30901;,
 -0.07821;-0.00974;-0.15349;,
 0.05324;-0.00974;-0.16384;,
 0.30902;-8.53812;-0.95106;,
 -0.45400;-8.53812;-0.89101;,
 0.15350;-0.00974;-0.07820;,
 0.89102;-8.53812;-0.45399;,
 0.15350;-0.00974;-0.07820;,
 0.16385;-0.00974;0.05325;,
 0.95107;-8.53812;0.30903;,
 0.89102;-8.53812;-0.45399;,
 0.07821;-0.00974;0.15351;,
 0.45400;-8.53812;0.89102;,
 0.07821;-0.00974;0.15351;,
 -0.05324;-0.00974;0.16386;,
 -0.30902;-8.53812;0.95108;,
 0.45400;-8.53812;0.89102;,
 -0.15350;-0.00974;0.07823;,
 -0.89102;-8.53812;0.45401;,
 -0.15350;-0.00974;0.07823;,
 -0.89102;-8.53812;0.45401;;
 
 8;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;10,11,12,13;,
 4;11,14,15,12;,
 4;16,17,18,19;,
 4;17,20,21,18;,
 4;22,0,3,23;;
 
 MeshMaterialList {
  1;
  8;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\3DGame\\2012_1_21\\çÏã∆íÜ\\Texture\\Spotlight.png";
   }
  }
 }
 MeshNormals {
  8;
  -0.946609;0.096603;-0.307570;,
  -0.451868;0.096603;-0.886839;,
  0.307571;0.096603;-0.946609;,
  0.886840;0.096603;-0.451866;,
  0.946608;0.096602;0.307574;,
  0.451867;0.096602;0.886840;,
  -0.307568;0.096602;0.946609;,
  -0.886837;0.096603;0.451872;;
  8;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,0,0,7;;
 }
 MeshTextureCoords {
  24;
  0.480490;0.015040;
  0.504530;0.015170;
  0.764070;0.968340;
  0.492650;0.975260;
  0.459300;0.012390;
  0.483330;0.012810;
  0.492590;0.978570;
  0.159840;0.976150;
  0.507360;0.013230;
  0.763980;0.973340;
  0.459320;0.014610;
  0.483350;0.015030;
  0.492710;0.975200;
  0.159970;0.972770;
  0.507380;0.015450;
  0.764100;0.969970;
  0.458450;0.016350;
  0.482190;0.016760;
  0.490150;0.976900;
  0.159100;0.974490;
  0.506800;0.017480;
  0.764890;0.973410;
  0.456450;0.014910;
  0.159870;0.974510;;
 }
}
