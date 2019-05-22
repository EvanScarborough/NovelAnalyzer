//
//  Global.hpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#ifndef Global_hpp
#define Global_hpp

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "ResourcePath.hpp"

using namespace std;
using namespace sf;



enum FormatType{ LOWER, SIMPLE };

struct WordCount{
	string word;
	int count;
	WordCount(string w, int c) : word(w),count(c) {}
	
	bool operator < (const WordCount& str) const{
		return count < str.count;
	}
	bool operator > (const WordCount& str) const{
		return count > str.count;
	}
};

struct WordFrequency{
	string word;
	float freq;
	WordFrequency(string w, float f) : word(w),freq(f) {}
	
	bool operator < (const WordFrequency& str) const{
		return freq < str.freq;
	}
	bool operator > (const WordFrequency& str) const{
		return freq > str.freq;
	}
};

class Global{
public:
	static Font font;
	static Text text;
	
	static void setup(){
		font.loadFromFile(resourcePath() + "sansation.ttf");
		text.setFont(font);
	}
	
	static int rnum(int max){
		int n = int((double(rand()) / RAND_MAX) * max);
		if(n == max) return 0;
		return n;
	}
	
	static void format(string& s, FormatType type){
		if(type == SIMPLE){
			for(int i = 0; i < s.size(); i++){
				if(isalpha(s.at(i))){
					if(isupper(s.at(i))){
						s.at(i) = tolower(s.at(i));
					}
				}
				else if(!isnumber(s.at(i)) && s.at(i) != '-'){
					s.erase(s.begin() + i);
					i--;
				}
			}
		}
	}
	
	static bool hasAlpha(string& s){
		for(int i = 0; i < s.size(); i++){
			if(isalpha(s.at(i))){
				return true;
			}
		}
		return false;
	}
	
	static void print(WordCount w){
		cout << w.word << "\t" << w.count << endl;
	}
	static void print(vector<WordCount>& v){
		for(int i = 0; i < v.size(); i++){
			cout << i << "\t";
			print(v.at(i));
		}
	}
	
	static float abs(float f){if(f<0)return f*-1;return f;}
	
	static int max(vector<int>& m){int ret = 0;for(int i=0;i<m.size();i++)if(m.at(i)>ret)ret=m.at(i);return ret;}
	static float max(vector<float>& m){float ret = 0;for(int i=0;i<m.size();i++)if(m.at(i)>ret)ret=m.at(i);return ret;}
	static int min(vector<int>& m){int ret = 0;for(int i=0;i<m.size();i++)if(m.at(i)<ret)ret=m.at(i);return ret;}
	static float min(vector<float>& m){float ret = 0;for(int i=0;i<m.size();i++)if(m.at(i)<ret)ret=m.at(i);return ret;}
	static int absMax(vector<int>& m){int m1=max(m);int m2=min(m);if(m2*-1>m1)return m2*-1;return m1;}
	static int absMax(vector<float>& m){float m1=max(m);float m2=min(m);if(m2*-1>m1)return m2*-1;return m1;}
	
	static string getAsWord(int n);
	static string getAsWordth(int n);
};



#endif /* Global_hpp */
