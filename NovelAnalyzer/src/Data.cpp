//
//  Data.cpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/27/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#include "Data.hpp"

bool Data::colorDataLoaded = false;
map<string,Color> Data::colorData = map<string,Color>();
void Data::loadColorData(){
	if(colorDataLoaded) return;
	ifstream fin(resourcePath() + "data/colors.txt");
	if(fin.fail()){
		cout << "Failed to load color data!" << endl;
		return;
	}
	string line = "";
	while(!fin.eof()){
		getline(fin,line);
		istringstream lin(line);
		string name = "";
		int r = 0;
		int g = 0;
		int b = 0;
		int a = 255;
		lin >> name;
		lin >> r;
		lin >> g;
		lin >> b;
		lin >> a;
		colorData.emplace(name,Color(r,g,b,a));
	}
	colorDataLoaded = true;
}


Color Data::mergeColors(vector<Color>& colors){
	if(colors.size()==0) return Color::Black;
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	int num = 0;
	for(int i = 0; i < colors.size(); i++){
		r += colors.at(i).r;
		g += colors.at(i).g;
		b += colors.at(i).b;
		a += colors.at(i).a;
		num++;
	}
	r = r/num; g = g/num; b = b/num; a = a/num;
	if(r > 255) r = 255; if(r < 0) r = 0;
	if(g > 255) g = 255; if(g < 0) g = 0;
	if(b > 255) b = 255; if(b < 0) b = 0;
	if(a > 255) a = 255; if(a < 0) a = 0;
	return Color(r,g,b,a);
}


Color Data::colorInterp(Color c1, Color c2, float interp){
	int r = c1.r + (c2.r - c1.r) * interp;
	int g = c1.g + (c2.g - c1.g) * interp;
	int b = c1.b + (c2.b - c1.b) * interp;
	int a = c1.a + (c2.a - c1.a) * interp;
	return Color(r,g,b,a);
}







bool Data::sentimentDataLoaded = false;
map<string,int> Data::sentimentData = map<string,int>();
void Data::loadSentimentData(){
	if(sentimentDataLoaded) return;
	ifstream fin(resourcePath() + "data/AFINN/AFINN-111.txt");
	if(fin.fail()){
		cout << "Failed to load feeling data!" << endl;
		return;
	}
	string line = "";
	while(!fin.eof()){
		getline(fin,line);
		istringstream lin(line);
		string word = "";
		int score = 0;
		lin >> word;
		lin >> score;
		sentimentData.emplace(word,score);
		//cout << word << " " << score << endl;
	}
	sentimentDataLoaded = true;
}






float Data::findCorrelation(vector<float>& x, vector<float>& y){
	int n = x.size();
	if(y.size() < n) n = y.size();
	float xbar = 0;
	float ybar = 0;
	for(int i = 0; i < n; i++){
		xbar += x.at(i);
		ybar += y.at(i);
	}
	xbar /= (float)n;
	ybar /= (float)n;
	float absum = 0;
	float a2sum = 0;
	float b2sum = 0;
	for(int i = 0; i < n; i++){
		float a = x.at(i) - xbar;
		float b = y.at(i) - ybar;
		absum += a*b;
		a2sum += a*a;
		b2sum += b*b;
	}
	absum /= (float)n;
	a2sum /= (float)n;
	b2sum /= (float)n;
	return absum / sqrt(a2sum * b2sum);
}

float Data::findBestFitSlope(vector<float>& x, vector<float>& y){
	int n = x.size();
	if(y.size() < n) n = y.size();
	float xbar = 0;
	float ybar = 0;
	for(int i = 0; i < n; i++){
		xbar += x.at(i);
		ybar += y.at(i);
	}
	xbar /= (float)n;
	ybar /= (float)n;
	float top = 0;
	float bottom = 0;
	for(int i = 0; i < n; i++){
		top += (x.at(i)-xbar) * (y.at(i)-ybar);
		bottom += (x.at(i)-xbar) * (x.at(i)-xbar);
	}
	//cout << top << " " << bottom << endl;
	return top/bottom;
}












