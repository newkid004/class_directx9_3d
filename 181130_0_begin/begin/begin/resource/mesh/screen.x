xof 0303txt 0032
template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Frame Model {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh {
  25;
  -0.999000;-0.999000;0.001000;,
  -0.499000;-0.999000;0.001000;,
  0.001000;-0.999000;0.001000;,
  0.501000;-0.999000;0.001000;,
  1.001000;-0.999000;0.001000;,
  -0.999000;-0.499000;0.001000;,
  -0.499000;-0.499000;0.001000;,
  0.001000;-0.499000;0.001000;,
  0.501000;-0.499000;0.001000;,
  1.001000;-0.499000;0.001000;,
  -0.999000;0.001000;0.001000;,
  -0.499000;0.001000;0.001000;,
  0.001000;0.001000;0.001000;,
  0.501000;0.001000;0.001000;,
  1.001000;0.001000;0.001000;,
  -0.999000;0.501000;0.001000;,
  -0.499000;0.501000;0.001000;,
  0.001000;0.501000;0.001000;,
  0.501000;0.501000;0.001000;,
  1.001000;0.501000;0.001000;,
  -0.999000;1.001000;0.001000;,
  -0.499000;1.001000;0.001000;,
  0.001000;1.001000;0.001000;,
  0.501000;1.001000;0.001000;,
  1.001000;1.001000;0.001000;;
  32;
  3;5,0,6;,
  3;1,6,0;,
  3;6,1,7;,
  3;2,7,1;,
  3;7,2,8;,
  3;3,8,2;,
  3;8,3,9;,
  3;4,9,3;,
  3;10,5,11;,
  3;6,11,5;,
  3;11,6,12;,
  3;7,12,6;,
  3;12,7,13;,
  3;8,13,7;,
  3;13,8,14;,
  3;9,14,8;,
  3;15,10,16;,
  3;11,16,10;,
  3;16,11,17;,
  3;12,17,11;,
  3;17,12,18;,
  3;13,18,12;,
  3;18,13,19;,
  3;14,19,13;,
  3;20,15,21;,
  3;16,21,15;,
  3;21,16,22;,
  3;17,22,16;,
  3;22,17,23;,
  3;18,23,17;,
  3;23,18,24;,
  3;19,24,18;;

  MeshNormals {
   25;
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;1.000000;;
   32;
   3;5,0,6;,
   3;1,6,0;,
   3;6,1,7;,
   3;2,7,1;,
   3;7,2,8;,
   3;3,8,2;,
   3;8,3,9;,
   3;4,9,3;,
   3;10,5,11;,
   3;6,11,5;,
   3;11,6,12;,
   3;7,12,6;,
   3;12,7,13;,
   3;8,13,7;,
   3;13,8,14;,
   3;9,14,8;,
   3;15,10,16;,
   3;11,16,10;,
   3;16,11,17;,
   3;12,17,11;,
   3;17,12,18;,
   3;13,18,12;,
   3;18,13,19;,
   3;14,19,13;,
   3;20,15,21;,
   3;16,21,15;,
   3;21,16,22;,
   3;17,22,16;,
   3;22,17,23;,
   3;18,23,17;,
   3;23,18,24;,
   3;19,24,18;;
  }

  MeshTextureCoords {
   25;
   0.000500;0.999500;,
   0.250250;0.999500;,
   0.500000;0.999500;,
   0.749750;0.999500;,
   0.999500;0.999501;,
   0.000500;0.749750;,
   0.250250;0.749750;,
   0.500000;0.749750;,
   0.749750;0.749750;,
   0.999500;0.749750;,
   0.000500;0.500000;,
   0.250250;0.500000;,
   0.500000;0.500000;,
   0.749750;0.500000;,
   0.999500;0.500000;,
   0.000500;0.250250;,
   0.250250;0.250250;,
   0.500000;0.250250;,
   0.749750;0.250250;,
   0.999500;0.250250;,
   0.000500;0.000499;,
   0.250250;0.000500;,
   0.500000;0.000500;,
   0.749750;0.000500;,
   0.999501;0.000500;;
  }
 }
}