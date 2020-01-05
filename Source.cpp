/*
IMAGE PROCESSING PROGRAM (C++)
=================================================================================================================
BY:				Sean Hernandez
CREATED:		10/29/2019
LAST MODIFIED:	11/10/2019
SYNOPSIS:		Functions will receive file paths in .tga format and modify them appropriately. 
				Functions include standard image processing operations commonly seen in programs like Photoshop
FOR:			COP3503 project 2
=================================================================================================================
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<random>

using namespace std;

struct Header
{
	Header(char idl, char cmt, char dtc, short cmo, short cml, char cmd, short xo, short yo, short w, short h, char bpp, char id) //dont roast me
	{
		idLength = idl;
		colorMapType = cmt;
		dataTypeCode = dtc;
		colorMapOrigin = cmo;
		colorMapLength = cml;
		colorMapDepth = cmd;
		xOrigin = xo;
		yOrigin = yo;
		width = w;
		height = h;
		bitsPerPixel = bpp;
		imageDescriptor = id;
	}

	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
};

struct Pixel
{
	Pixel(unsigned char red, unsigned char green, unsigned char blue)
	{
		R = red;
		G = green;
		B = blue;
	}
	unsigned char R;
	unsigned char G;
	unsigned char B;
};


Header readHeader(string& path);
vector<Pixel> readImage(string& path);
void writeFile(Header& header, string& destination);
void printHeaderInfo(string& path);
bool identical(string& path1, string& path2);
void dupeFile(string& path);
void rainbow(string& path);
void multiply(string& path1, string& path2, string& destination);
void subtract(string& target, string& source, string& destination);
void screen(string& top, string& bottom, string& destination);
void overlay(string& path1, string& path2, string& destination);
void invert(string& path);
void linearBurn(string& top, string& bottom, string& destination);
void addColor(string& path, string& channel, int& amount, string& destination);
void scaleColor(string& path, string& channel, int& amount, string& destination);
void extractColor(string& path, string& color, string& destination);
void combineChannels(string& channel_r, string& channel_g, string& channel_b, string& destination);
void rotate(string& path, string& destination);
void stitch(string& path1, string& path2, string& path3, string& path4, string& destination);

int main()
{

	//test 1
	string target = "./input/layer1.tga";
	string source = "./input/pattern1.tga";
	string ans = "./output/part1.tga";
	string key = "./input/EXAMPLE_part1.tga";
	multiply(target, source, ans);
	if (identical(ans, key) == 1) { cout << "Test 1:\t\tpassed" << endl; }
	else cout << "Test 1:\t\tfailed" << endl;

	//test 2
	target = "./input/car.tga";
	source = "./input/layer2.tga";
	ans = "./output/part2.tga";
	subtract(target, source, ans);
	key = "./input/EXAMPLE_part2.tga";
	if (identical(ans, key) == 1) { cout << "Test 2:\t\tpassed" << endl; }
	else cout << "Test 2:\t\tfailed" << endl;

	//test 3
	string temp = "./output/temp.tga";
	target = "./input/layer1.tga";
	source = "./input/pattern2.tga";
	multiply(target, source, temp);
	target = "./input/text.tga";
	ans = "./output/part3.tga";
	screen(target, temp, ans);
	key = "./input/EXAMPLE_part3.tga";
	if (identical(ans, key) == 1) { cout << "Test 3:\t\tpassed" << endl;  }
	else cout << "Test 3:\t\tfailed" << endl;

	//test 4
	target = "./input/layer2.tga";
	source = "./input/circles.tga";
	multiply(target, source, temp);
	target = "./input/pattern2.tga";
	ans = "./output/part4.tga";
	key = "./input/EXAMPLE_part4.tga";
	subtract(temp, target, ans);
	if (identical(ans, key) == 1) cout << "Test 4:\t\tpassed" << endl;
	else cout << "Test 4:\t\tfailed" << endl;

	//test 5
	target = "./input/layer1.tga";
	source = "./input/pattern1.tga";
	ans = "./output/part5.tga";
	key = "./input/EXAMPLE_part5.tga";
	overlay(target, source, ans);
	if (identical(ans, key) == 1) cout << "Test 5:\t\tpassed" << endl;
	else cout << "Test 5:\t\tfailed" << endl;

	//test 6
	string channel = "./input/green";
	target = "./input/car.tga";
	ans = "./output/part6.tga";
	key = "./input/EXAMPLE_part6.tga";
	int x = 200;
	addColor(target, channel, x, ans);
	if (identical(ans, key) == 1) cout << "Test 6:\t\tpassed" << endl;
	else cout << "Test 6:\t\tfailed" << endl;

	//test 7
	target = "./input/car.tga";
	channel = "red";
	x = 4;
	scaleColor(target, channel, x, temp);
	ans = "./output/part7.tga";
	key = "./input/EXAMPLE_part7.tga";
	channel = "blue";
	x = 0;
	scaleColor(temp, channel, x, ans);
	if (identical(ans, key) == 1) cout << "Test 7:\t\tpassed" << endl;
	else cout << "Test 7:\t\tfailed" << endl;

	//test 8
	target = "./input/car.tga";
	channel = "green";
	string ans_g = "./output/part8_g.tga";
	string ans_b = "./output/part8_b.tga";
	string ans_r = "./output/part8_r.tga";
	extractColor(target, channel, ans_g);
	channel = "blue";
	extractColor(target, channel, ans_b);
	channel = "red";
	extractColor(target, channel, ans_r);
	string key_g = "./input/EXAMPLE_part8_g.tga";
	string key_b = "./input/EXAMPLE_part8_b.tga";
	string key_r = "./input/EXAMPLE_part8_r.tga";
	if (identical(ans_g, key_g) && identical(ans_r, key_r) && identical(ans_b, key_b)) cout << "Test 8:\t\tpassed" << endl;
	else cout << "Test 8:\t\tfailed" << endl;

	//test 9
	ans_g = "./input/layer_green.tga";
	ans_b = "./input/layer_blue.tga";
	ans_r = "./input/layer_red.tga";
	ans = "./output/part9.tga";
	key = "./input/EXAMPLE_part9.tga";
	combineChannels(ans_r, ans_g, ans_b, ans);
	if (identical(ans, key) == 1) cout << "Test 9:\t\tpassed" << endl;
	else cout << "Test 9:\t\tfailed" << endl;

	//test 10
	target = "./input/text2.tga";
	ans = "./output/part10.tga";
	key = "./input/EXAMPLE_part10.tga";
	rotate(target, ans);
	if (identical(ans, key) == 1) cout << "Test 10:\tpassed" << endl;
	else cout << "Test 10:\t\tfailed" << endl;

	//extra credit
	target = "./input/text.tga";
	source = "./input/pattern1.tga";
	temp = "./input/car.tga";
	channel = "./input/circles.tga";
	ans = "./output/extracredit.tga";
	key = "./input/EXAMPLE_extracredit.tga";
	stitch(target, source, temp, channel, ans);
	if (identical(ans, key) == 1) cout << "Extra credit:\tpassed" << endl;
	else cout << "Test 11:\t\tfailed" << endl;
}

bool identical(string& path1, string& path2)
{
	vector<Pixel> pixel1 = readImage(path1);
	vector<Pixel> pixel2 = readImage(path2);


	for (int i = 0; i < pixel1.size(); i++)
	{
		if (pixel1.at(i).B == pixel2.at(i).B && pixel1.at(i).G == pixel2.at(i).G && pixel1.at(i).R == pixel2.at(i).R)
		{
			continue;
		}
		else
		{
			//cout << path1 << " and " << path2 << " are not identical images" << endl;
			return false;
		}
	}

	//cout << path1 << " and " << path2 << " are identical images" << endl;
	return true;
}

void invert(string& path)
{
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);

	cout << endl << "Inverting! ... " << endl;
	fstream file("inverted.tga", ios_base::out | ios_base::binary);

	if (file.is_open())
	{
		//write the header
		char idLength_ = header.idLength;
		file.write(&idLength_, 1);

		char colorType_ = header.colorMapType;
		file.write(&colorType_, 1);

		char dataType_ = header.dataTypeCode;
		file.write(&dataType_, 1);

		short colorOrigin_ = header.colorMapOrigin;
		file.write((char*)&colorOrigin_, 2);

		short colorLength_ = header.colorMapLength;
		file.write((char*)&colorLength_, 2);

		char colorDepth_ = header.colorMapDepth;
		file.write((char*)&colorDepth_, 1);

		short xOrigin = header.xOrigin;
		file.write((char*)&xOrigin, 2);

		short yOrigin = header.yOrigin;
		file.write((char*)&yOrigin, 2);

		short width = header.width;
		file.write((char*)&width, 2);

		short height = header.height;
		file.write((char*)&height, 2);

		char bits_ = header.bitsPerPixel;
		file.write(&bits_, 1);

		char imageDesc_ = header.imageDescriptor;
		file.write(&imageDesc_, 1);

		for (Pixel& p : pixels)
		{
			unsigned char newBlue = static_cast<unsigned int>(255 - (int)p.B);
			unsigned char newGreen = static_cast<unsigned int>(255 - (int)p.G);
			unsigned char newRed = static_cast<unsigned int>(255 - (int)p.R);

			file.write((char*)&newBlue, 1 * sizeof(newBlue));
			file.write((char*)&newGreen, 1 * sizeof(newGreen));
			file.write((char*)&newRed, 1 * sizeof(newRed));
		}


	}
}

void printHeaderInfo(string& path)
{
	Header header = readHeader(path);
	cout << "FILE NAME: " << path << endl << endl;
	cout << "ID Length: " << header.idLength << endl;
	cout << "Color Map Type: " << header.colorMapType << endl;
	cout << "Data Type Code: " << header.dataTypeCode << endl;
	cout << "Color Map Origin: " << header.colorMapOrigin << endl;
	cout << "Color Map Length: " << header.colorMapLength << endl;
	cout << "Color Map Depth " << header.colorMapDepth << endl;
	cout << "x Origin: " << header.xOrigin << endl;
	cout << "y Origin: " << header.yOrigin << endl;
	cout << "Width: " << header.width << endl;
	cout << "Height: " << header.height << endl;
	cout << "Bits / Pixel: " << header.bitsPerPixel << endl;
	cout << "Image descriptor: " << header.imageDescriptor << endl;
	cout << endl;
}

Header readHeader(string& path)
{
	ifstream file(path, ios_base::in | ios_base::binary);

	//read the header

	char idLength;
	file.read(&idLength, 1);

	char colorMapType;
	file.read(&colorMapType, 1);

	char dataTypeCode;
	file.read(&dataTypeCode, 1);

	short colorMapOrigin;
	file.read((char*)&colorMapOrigin, 2);

	short colorMapLength;
	file.read((char*)&colorMapLength, 2);

	char colorMapDepth;
	file.read(&colorMapDepth, 1);

	short xOrigin;
	file.read((char*)&xOrigin, 2);

	short yOrigin;
	file.read((char*)&yOrigin, 2);

	short width;
	file.read((char*)&width, 2);

	short height;
	file.read((char*)&height, 2);

	char bitsPerPixel;
	file.read(&bitsPerPixel, 1);

	char imageDescriptor;
	file.read(&imageDescriptor, 1);

	Header header(idLength, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth, xOrigin, yOrigin, width, height, bitsPerPixel, imageDescriptor);
	return header;
}

vector<Pixel> readImage(string& path)
{
	vector<Pixel> pixels;

	Header header = readHeader(path);

	ifstream file(path, ios_base::in | ios_base::binary);

	file.seekg(18);		//skips past header to image data


	for (int i = 0; i < header.width * header.height; i++)
	{
		unsigned char blue;
		file.read((char*)&blue, 1 * sizeof(blue));

		unsigned char green;
		file.read((char*)&green, 1 * sizeof(green));

		unsigned char red;
		file.read((char*)&red, 1 * sizeof(red));

		Pixel p = Pixel(red, green, blue);

		pixels.push_back(p);
	}
	return pixels;
}

void writeFile(Header& header, string& destination)
{
	//writes the header of a file

	fstream file(destination, ios_base::out | ios_base::binary);

	char idLength_ = header.idLength;
	file.write(&idLength_, 1);

	char colorType_ = header.colorMapType;
	file.write(&colorType_, 1);

	char dataType_ = header.dataTypeCode;
	file.write(&dataType_, 1);

	short colorOrigin_ = header.colorMapOrigin;
	file.write((char*)&colorOrigin_, 2);

	short colorLength_ = header.colorMapLength;
	file.write((char*)&colorLength_, 2);

	char colorDepth_ = header.colorMapDepth;
	file.write((char*)&colorDepth_, 1);

	short xOrigin = header.xOrigin;
	file.write((char*)&xOrigin, 2);

	short yOrigin = header.yOrigin;
	file.write((char*)&yOrigin, 2);

	short width = header.width;
	file.write((char*)&width, 2);

	short height = header.height;
	file.write((char*)&height, 2);

	char bits_ = header.bitsPerPixel;
	file.write(&bits_, 1);

	char imageDesc_ = header.imageDescriptor;
	file.write(&imageDesc_, 1);
}

void dupeFile(string& path)
{
	cout << endl << "Duping Files..." << endl << endl;
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);


	fstream file("car_prime.tga", ios_base::out | ios_base::binary);

	if (file.is_open())
	{
		//write the header
		char idLength_ = header.idLength;
		file.write(&idLength_, 1);

		char colorType_ = header.colorMapType;
		file.write(&colorType_, 1);

		char dataType_ = header.dataTypeCode;
		file.write(&dataType_, 1);

		short colorOrigin_ = header.colorMapOrigin;
		file.write((char*)&colorOrigin_, 2);

		short colorLength_ = header.colorMapLength;
		file.write((char*)&colorLength_, 2);

		char colorDepth_ = header.colorMapDepth;
		file.write((char*)&colorDepth_, 1);

		short xOrigin = header.xOrigin;
		file.write((char*)&xOrigin, 2);

		short yOrigin = header.yOrigin;
		file.write((char*)&yOrigin, 2);

		short width = header.width;
		file.write((char*)&width, 2);

		short height = header.height;
		file.write((char*)&height, 2);

		char bits_ = header.bitsPerPixel;
		file.write(&bits_, 1);

		char imageDesc_ = header.imageDescriptor;
		file.write(&imageDesc_, 1);

		//write the image

		for (Pixel& p : pixels)
		{
			unsigned int blue = p.B;
			file.write((char*)&blue, 1);

			unsigned char green = p.G;
			file.write((char*)&green, 1);

			unsigned char red = p.R;
			file.write((char*)&red, 1);

		}
	}


}

void rainbow(string& path)
{
	cout << endl << "Randomizing Colors..." << endl << endl;

	Header header = readHeader(path);

	fstream file(path, ios_base::out | ios_base::binary);


	//write the header
	char idLength_ = header.idLength;
	file.write(&idLength_, 1);

	char colorType_ = header.colorMapType;
	file.write(&colorType_, 1);

	char dataType_ = header.dataTypeCode;
	file.write(&dataType_, 1);

	short colorOrigin_ = header.colorMapOrigin;
	file.write((char*)&colorOrigin_, 2);

	short colorLength_ = header.colorMapLength;
	file.write((char*)&colorLength_, 2);

	char colorDepth_ = header.colorMapDepth;
	file.write((char*)&colorDepth_, 1);

	short xOrigin = header.xOrigin;
	file.write((char*)&xOrigin, 2);

	short yOrigin = header.yOrigin;
	file.write((char*)&yOrigin, 2);

	short width = header.width;
	file.write((char*)&width, 2);

	short height = header.height;
	file.write((char*)&height, 2);

	char bits_ = header.bitsPerPixel;
	file.write(&bits_, 1);

	char imageDesc_ = header.imageDescriptor;
	file.write(&imageDesc_, 1);

	mt19937 mt(1729);
	uniform_int_distribution<int> dist1(0, 255);
	uniform_int_distribution<int> dist2(0, 255);
	uniform_int_distribution<int> dist3(0, 255);

	for (int i = 0; i < header.width * header.height; i++)
	{

		unsigned char blue = static_cast<unsigned char>(dist1(mt));
		unsigned char green = static_cast<unsigned char>(dist2(mt));
		unsigned char red = static_cast<unsigned char>(dist3(mt));

		file.write((char*)&blue, 1 * sizeof(blue));
		file.write((char*)&green, 1 * sizeof(green));
		file.write((char*)&red, 1 * sizeof(red));
	}

}

void multiply(string& path1, string& path2, string& destination)
{

	Header header1 = readHeader(path1);
	Header header2 = readHeader(path2);

	writeFile(header1, destination);

	vector<Pixel> pixels1 = readImage(path1);
	vector<Pixel> pixels2 = readImage(path2);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18); //skip past the header since its already written

	for (int i = 0; i < header1.width * header1.height; i++)
	{
		unsigned char newBlue = (unsigned char)((float)pixels1.at(i).B * (float)pixels2.at(i).B / 255 + 0.5f);
		unsigned char newGreen = (unsigned char)((float)pixels1.at(i).G * (float)pixels2.at(i).G / 255 + 0.5f);
		unsigned char newRed = (unsigned char)((float)pixels1.at(i).R * (float)pixels2.at(i).R / 255 + 0.5f);

		file.write((char*)&newBlue, sizeof(newBlue));
		file.write((char*)&newGreen, sizeof(newGreen));
		file.write((char*)&newRed, sizeof(newRed));
	}
}

void subtract(string& target, string& source, string& destination)
{

	Header header1 = readHeader(target);
	Header header2 = readHeader(source);

	writeFile(header1, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<Pixel> pixels1 = readImage(target);
	vector<Pixel> pixels2 = readImage(source);

	for (int i = 0; i < pixels1.size(); i++)
	{
		unsigned char newBlue;
		unsigned char newGreen;
		unsigned char newRed;

		//subtraction cant ever be > 255 so dont bother with that case
		if ((int)pixels1.at(i).B - (int)pixels2.at(i).B > 0)
		{
			newBlue = (unsigned char)((int)pixels1.at(i).B - (int)pixels2.at(i).B);
		}
		else
		{
			newBlue = (unsigned char)0;
		}
		if ((int)pixels1.at(i).G - (int)pixels2.at(i).G > 0)
		{
			newGreen = (unsigned char)((int)pixels1.at(i).G - (int)pixels2.at(i).G);
		}
		else
		{
			newGreen = (unsigned char)0;
		}
		if ((int)pixels1.at(i).R - (int)pixels2.at(i).R > 0)
		{
			newRed = (unsigned char)((int)pixels1.at(i).R - (int)pixels2.at(i).R);
		}
		else
		{
			newRed = (unsigned char)0;
		}

		file.write((char*)&newBlue, sizeof(newBlue));
		file.write((char*)&newGreen, sizeof(newGreen));
		file.write((char*)&newRed, sizeof(newRed));
	}
}

void screen(string& top, string& bottom, string& destination)
{


	Header header1 = readHeader(bottom);
	Header header2 = readHeader(top);

	writeFile(header1, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<Pixel> pixels1 = readImage(top);
	vector<Pixel> pixels2 = readImage(bottom);

	for (int i = 0; i < pixels1.size(); i++)
	{
		unsigned char newBlue = (unsigned char)(0.5f + 255 - (float)(255 - pixels1.at(i).B) * (float)(255 - pixels2.at(i).B) / 255);
		unsigned char newGreen = (unsigned char)(0.5f + 255 - (float)(255 - pixels1.at(i).G) * (float)(255 - pixels2.at(i).G) / 255);
		unsigned char newRed = (unsigned char)(0.5f + 255 - (float)(255 - pixels1.at(i).R) * (float)(255 - pixels2.at(i).R) / 255);

		file.write((char*)&newBlue, 1 * sizeof(newBlue));
		file.write((char*)&newGreen, 1 * sizeof(newGreen));
		file.write((char*)&newRed, 1 * sizeof(newRed));
	}

}

void overlay(string& top, string& bottom, string& destination)
{

	Header header1 = readHeader(top);
	Header header2 = readHeader(bottom);

	writeFile(header1, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<Pixel> pixels1 = readImage(top);
	vector<Pixel> pixels2 = readImage(bottom);

	for (int i = 0; i < pixels2.size(); i++)
	{
		if ((float)pixels2.at(i).B / 255 <= 0.5f && (float)pixels2.at(i).G / 255 <= 0.5f && (float)pixels2.at(i).G / 255 <= 0.5f)
		{
			unsigned char newBlue = (unsigned char)(2 * (float)pixels1.at(i).B * (float)pixels2.at(i).B / 255 + 0.5f);
			unsigned char newGreen = (unsigned char)(2 * (float)pixels1.at(i).G * (float)pixels2.at(i).G / 255 + 0.5f);
			unsigned char newRed = (unsigned char)(2 * (float)pixels1.at(i).R * (float)pixels2.at(i).R / 255 + 0.5f);

			file.write((char*)&newBlue, 1 * sizeof(newBlue));
			file.write((char*)&newGreen, 1 * sizeof(newGreen));
			file.write((char*)&newRed, 1 * sizeof(newRed));
		}
		else if ((float)pixels2.at(i).B / 255 > 0.5f && (float)pixels2.at(i).G / 255 > 0.5f && (float)pixels2.at(i).G / 255 > 0.5f)
		{
			unsigned char newBlue = (unsigned char)(0.5f + 255 * (1 - 2 * (1 - (float)pixels1.at(i).B / 255) * (1 - (float)pixels2.at(i).B / 255)));
			unsigned char newGreen = (unsigned char)(0.5f + 255 * (1 - 2 * (1 - (float)pixels1.at(i).G / 255) * (1 - (float)pixels2.at(i).G / 255)));
			unsigned char newRed = (unsigned char)(0.5f + 255 * (1 - 2 * (1 - (float)pixels1.at(i).R / 255) * (1 - (float)pixels2.at(i).R / 255)));

			file.write((char*)&newBlue, 1 * sizeof(newBlue));
			file.write((char*)&newGreen, 1 * sizeof(newGreen));
			file.write((char*)&newRed, 1 * sizeof(newRed));
		}
	}



}

void linearBurn(string& top, string& bottom, string& destination)
{
	cout << endl << "Burning! ..." << endl;

	Header header = readHeader(top);

	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<Pixel> pixels1 = readImage(top);
	vector<Pixel> pixels2 = readImage(bottom);

	for (int i = 0; i < pixels1.size(); i++)
	{
		unsigned char newBlue = (unsigned char)((int)pixels1.at(i).B + (int)pixels2.at(i).B - 255);
		unsigned char newRed = (unsigned char)((int)pixels1.at(i).R + (int)pixels2.at(i).R - 255);
		unsigned char newGreen = (unsigned char)((int)pixels1.at(i).G + (int)pixels2.at(i).G - 255);

		file.write((char*)&newBlue, sizeof(newBlue));
		file.write((char*)&newRed, sizeof(newRed));
		file.write((char*)&newGreen, sizeof(newGreen));
	}

}

void addColor(string& path, string& channel, int& amount, string& destination)
{
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);

	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	for (Pixel p : pixels) //<- iterates through every pixel, each 3 bytes
	{
		unsigned char newBlue;
		unsigned char newGreen;
		unsigned char newRed;

		if (channel == "green")
		{
			newBlue = p.B;
			newRed = p.R;

			file.write((char*)&newBlue, sizeof(newBlue));

			if ((int)p.G + amount <= 255)
			{
				newGreen = (unsigned char)((int)p.G + amount);
				file.write((char*)&newGreen, sizeof(newGreen));
			}
			else if (255 < (int)p.G + amount)
			{
				newGreen = (unsigned char)255;
				file.write((char*)&newGreen, sizeof(newGreen));
			}

			file.write((char*)&newRed, sizeof(newRed));
		}
		else if (channel == "blue")
		{
			newGreen = p.G;
			newRed = p.R;

			if ((int)p.B + amount <= 255)
			{
				newBlue = (unsigned char)((int)p.B + amount);
				file.write((char*)&newBlue, sizeof(newBlue));
			}
			else if (255 < (int)p.G + amount)
			{
				newGreen = (unsigned char)255;
				file.write((char*)&newBlue, sizeof(newBlue));
			}

			file.write((char*)&newGreen, sizeof(newGreen));

			file.write((char*)&newRed, sizeof(newRed));
		}
		else if (channel == "red")
		{
			newBlue = p.B;
			newGreen = p.G;

			file.write((char*)&newBlue, sizeof(newBlue));

			file.write((char*)&newGreen, sizeof(newGreen));

			if ((int)p.R + amount <= 255)
			{
				newRed = (unsigned char)((int)p.R + amount);
				file.write((char*)&newRed, sizeof(newRed));
			}
			else if (255 < (int)p.R + amount)
			{
				newRed = (unsigned char)255;
				file.write((char*)&newRed, sizeof(newRed));
			}
		}
	}

}

void scaleColor(string& path, string& channel, int& amount, string& destination)
{
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);

	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	for (Pixel p : pixels)
	{
		if (channel == "green")
		{
			unsigned char newBlue = p.B;
			unsigned char newRed = p.R;
			unsigned char newGreen;

			file.write((char*)&newBlue, sizeof(newBlue));

			float result = amount * (int)p.G;

			if (result <= 255)
			{
				newGreen = (unsigned char)(result + 0.5f);
				file.write((char*)&newGreen, sizeof(newGreen));
			}
			else if (255 < result)
			{
				newGreen = (unsigned char)255;
				file.write((char*)&newGreen, sizeof(newGreen));
			}
			file.write((char*)&newRed, sizeof(newRed));
		}
		else if (channel == "blue")
		{
			unsigned char newGreen = p.G;
			unsigned char newRed = p.R;
			unsigned char newBlue;

			int result = amount * (int)p.B;

			if (result < 255)
			{
				newBlue = (unsigned char)result;
				file.write((char*)&newBlue, sizeof(newBlue));
			}
			else if (255 < result)
			{
				newBlue = (unsigned char)result;
				file.write((char*)&newBlue, sizeof(newBlue));
			}

			file.write((char*)&newGreen, sizeof(newGreen));
			file.write((char*)&newRed, sizeof(newRed));
		}
		else if (channel == "red")
		{
			unsigned char newBlue = p.B;
			unsigned char newGreen = p.G;
			unsigned char newRed;

			file.write((char*)&newBlue, sizeof(newBlue));
			file.write((char*)&newGreen, sizeof(newGreen));

			float result = amount * (int)p.R;

			if (result <= 255)
			{
				newRed = (unsigned char)(result + 0.5f);
				file.write((char*)&newRed, sizeof(newRed));
			}
			else if (255 < result)
			{
				newRed = (unsigned char)255;
				file.write((char*)&newRed, sizeof(newRed));
			}
		}
	}
}

void extractColor(string& path, string& color, string& destination)
{
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);
	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	if (color == "blue")
	{
		if (file.is_open())
		{
			//write the blue channel only
			for (Pixel& p : pixels)
			{
				unsigned char blue = p.B;

				file.write((char*)&blue, sizeof(blue));
				file.write((char*)&blue, sizeof(blue));
				file.write((char*)&blue, sizeof(blue));
			}
		}
	}
	else if (color == "green")
	{
		if (file.is_open())
		{
			for (Pixel& p : pixels)
			{
				unsigned char green = p.G;
				file.write((char*)&green, sizeof(green));
				file.write((char*)&green, sizeof(green));
				file.write((char*)&green, sizeof(green));

			}
		}
	}
	else if (color == "red")
	{
		if (file.is_open())
		{
			for (Pixel& p : pixels)
			{
				unsigned char red = p.R;
				file.write((char*)&red, sizeof(red));
				file.write((char*)&red, sizeof(red));
				file.write((char*)&red, sizeof(red));

			}
		}
	}
}

void combineChannels(string& channel_r, string& channel_g, string& channel_b, string& destination)
{
	//take any header
	Header header = readHeader(channel_r);
	vector<Pixel> pixels_r = readImage(channel_r);
	vector<Pixel> pixels_g = readImage(channel_g);
	vector<Pixel> pixels_b = readImage(channel_b);

	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	for (int i = 0; i < header.height * header.width; i++)
	{
		unsigned char newBlue = pixels_b.at(i).B;
		unsigned char newGreen = pixels_g.at(i).G;
		unsigned char newRed = pixels_r.at(i).R;

		file.write((char*)&newBlue, sizeof(newBlue));
		file.write((char*)&newGreen, sizeof(newGreen));
		file.write((char*)&newRed, sizeof(newRed));
	}
}

void rotate(string& path, string& destination)
{
	Header header = readHeader(path);
	vector<Pixel> pixels = readImage(path);

	writeFile(header, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<vector<Pixel>> rows; //yikes
	for (int i = 0; i < header.height; i++)
	{
		//create subsets of Pixels, each being a row
		vector<Pixel>::const_iterator first = pixels.begin() + i * header.width;
		vector<Pixel>::const_iterator last = pixels.begin() + (i * header.width + header.width);
		vector<Pixel> row(first, last);
		rows.push_back(row);
	}
	//iterate backwards through rows
	for (int j = rows.size() - 1; 0 <= j; j--)
	{
		//iterate backwards through Pixels in rows
		for (int i = rows.at(j).size() - 1; 0 <= i; i--)
		{
			unsigned char blue = rows.at(j).at(i).B;
			unsigned char green = rows.at(j).at(i).G;
			unsigned char red = rows.at(j).at(i).R;

			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}
	}
}

void stitch(string& path1, string& path2, string& path3, string& path4, string& destination)
{
	Header header1 = readHeader(path1);

	writeFile(header1, destination);

	fstream file(destination, ios_base::out | ios_base::binary);
	file.seekg(18);

	vector<Pixel> pixels1 = readImage(path1);
	vector<Pixel> pixels2 = readImage(path2);
	vector<Pixel> pixels3 = readImage(path3);
	vector<Pixel> pixels4 = readImage(path4);


	for (int i = 0; i < header1.height; i++)
	{
		for (int j = 0; j < header1.width; j++)
		{
			//write first line of first image
			unsigned char blue = pixels1.at(i * header1.height + j).B;
			unsigned char green = pixels1.at(i * header1.height + j).G;
			unsigned char red = pixels1.at(i * header1.height + j).R;
			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}
		for (int j = 0; j < header1.width; j++)
		{
			//write first line of second image
			unsigned char blue = pixels2.at(i * header1.height + j).B;
			unsigned char green = pixels2.at(i * header1.height + j).G;
			unsigned char red = pixels2.at(i * header1.height + j).R;
			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}
	}

	for (int i = 0; i < header1.height; i++)
	{
		for (int j = 0; j < header1.width; j++)
		{
			unsigned char blue = pixels3.at(i * header1.height + j).B;
			unsigned char green = pixels3.at(i * header1.height + j).G;
			unsigned char red = pixels3.at(i * header1.height + j).R;
			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}
		for (int j = 0; j < header1.width; j++)
		{
			//write first line of second image
			unsigned char blue = pixels4.at(i * header1.height + j).B;
			unsigned char green = pixels4.at(i * header1.height + j).G;
			unsigned char red = pixels4.at(i * header1.height + j).R;
			file.write((char*)&blue, sizeof(blue));
			file.write((char*)&green, sizeof(green));
			file.write((char*)&red, sizeof(red));
		}
	}
}