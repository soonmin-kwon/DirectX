#include <iostream>
#include <fstream>
using namespace std;

typedef struct {
	float x, y, z;
}VertexType;

typedef struct {
	int vIndex1, vIndex2, vIndex3, vIndex4;
	int tIndex1, tIndex2, tIndex3, tIndex4;
	int nIndex1, nIndex2, nIndex3, nIndex4;
}FaceType;

void GetModelFileName(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

int main() {
	bool result;
	char fileName[256];
	int vertexCnt, textureCnt, normalCnt, faceCnt;
	char garbage;
	
	GetModelFileName(fileName);

	result = ReadFileCounts(fileName, vertexCnt, textureCnt, normalCnt, faceCnt);
	if (!result) {
		return -1;
	}

	cout << "\n";
	cout << "Vertices : " << vertexCnt << "\n";
	cout << "Textures : " << textureCnt << "\n";
	cout << "Normals : " << normalCnt << "\n";
	cout << "Faces : " << faceCnt << "\n";

	result = LoadDataStructures(fileName, vertexCnt, textureCnt, normalCnt, faceCnt);
	if (!result) {
		return -1;
	}

	cout << "\nFile has been converted." << endl;
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}

void GetModelFileName(char* fileName) {
	bool done;
	ifstream fin;

	done = false;
	while (!done) {
		cout << "Enter model file name : ";
		cin >> fileName;
		fin.open(fileName);
		if (fin.good()) {
			done = true;
		}
		else {
			fin.clear();
			cout << "\n";
			cout << "File" << fileName << " could not be opend." << "\n\n";
		}
	}

	return;
}

bool ReadFileCounts(char* fileName , int& vertexCnt, int& textureCnt, int& normalCnt, int& faceCnt) {
	ifstream fin;
	char input;

	vertexCnt = 0;
	textureCnt = 0;
	normalCnt = 0;
	faceCnt = 0;

	fin.open(fileName);

	if (fin.fail()) {
		return false;
	}

	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);
			if (input == ' ') {
				vertexCnt++;
			}
			if (input == 't') {
				textureCnt++;
			}
			if (input == 'n') {
				normalCnt++;
			}
		}

		if (input == 'f') {
			fin.get(input);
			if (input == ' ') {
				faceCnt++;
			}
		}

		while (input != '\n') {
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}

bool LoadDataStructures(char* fileName, int vertexCnt, int textureCnt, int normalCnt, int faceCnt) {
	VertexType* vertices;
	VertexType* texcoords;
	VertexType* normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	vertices = new VertexType[vertexCnt];
	if (!vertices) {
		return false;
	}

	texcoords = new VertexType[textureCnt];
	if (!texcoords) {
		return false;
	}

	normals = new VertexType[normalCnt];
	if (!normals) {
		return false;
	}

	faces = new FaceType[faceCnt];
	if (!faces) {
		return false;
	}

	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	fin.open(fileName);

	if (fin.fail()) {
		return false;
	}

	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);

			if (input == ' ') {
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
				//vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			if (input == 't') {
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			if (input == 'n') {
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				//normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		if (input == 'f') {
			fin.get(input);
			if (input == ' ') {
				fin >> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex4 >> input2 >> faces[faceIndex].tIndex4 >> input2 >> faces[faceIndex].nIndex4;
				faceIndex++;
			}
		}

		while (input != '\n'){
			fin.get(input);
		}
		fin.get(input);
	}
	fin.close();

	fout.open("model.txt");

	fout << "Vertex Count: " << (faceCnt * 3) << "\n";
	fout << "\n";
	fout << "Data:" << "\n";
	fout << "\n";

	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";

		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";

		vIndex = faces[i].vIndex3 -1;
		tIndex = faces[i].tIndex3 -1;
		nIndex = faces[i].nIndex3 -1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";

		vIndex = faces[i].vIndex4 - 1;
		tIndex = faces[i].tIndex4 - 1;
		nIndex = faces[i].nIndex4 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << "\n";
	}
	fout.close();

	if (vertices) {
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords) {
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals) {
		delete[] normals;
		normals = 0;
	}

	if (faces) {
		delete[] faces;
		faces = 0;
	}

	return true;
}