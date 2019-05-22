//
//  Data.hpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/27/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>
#include "Global.hpp"



struct ColorMarker{
	int wordNum;
	Color color;
	ColorMarker(int w, Color c) : wordNum(w), color(c) {}
	bool operator < (const ColorMarker& str) const{
		return wordNum < str.wordNum;
	}
	bool operator > (const ColorMarker& str) const{
		return wordNum > str.wordNum;
	}
};


class Data{
public:
	
	
	static map<string,Color> colorData;
	static void loadColorData();
	static Color mergeColors(vector<Color>& colors);
	static Color colorInterp(Color c1, Color c2, float interp);
	
	
	static map<string,int> sentimentData;
	static void loadSentimentData();
	
	static float findCorrelation(vector<float>& d1, vector<float>& d2);
	static float findBestFitSlope(vector<float>& d1, vector<float>& d2);
	
private:
	static bool colorDataLoaded;
	static bool sentimentDataLoaded;
};






#endif /* Data_hpp */
