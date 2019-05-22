//
//  Global.cpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#include "Global.hpp"

Font Global::font = Font();
Text Global::text = Text();




string Global::getAsWord(int n){
	string tensNames[] = {
		"",
		" Ten",
		" Twenty",
		" Thirty",
		" Forty",
		" Fifty",
		" Sixty",
		" Seventy",
		" Eighty",
		" Ninety"
	};
	
	string numNames[] = {
		"",
		" One",
		" Two",
		" Three",
		" Four",
		" Five",
		" Six",
		" Seven",
		" Eight",
		" Nine",
		" Ten",
		" Eleven",
		" Twelve",
		" Thirteen",
		" Fourteen",
		" Fifteen",
		" Sixteen",
		" Seventeen",
		" Eighteen",
		" Nineteen"
	};
	
	
	string soFar;
	
	if(n % 100 < 20){
		soFar = numNames[n % 100];
		n /= 100;
	}
	else {
		soFar = numNames[n % 10];
		n /= 10;
		
		soFar = tensNames[n % 10] + soFar;
		n /= 10;
	}
	string ret;
	if (n == 0) ret = soFar;
	else ret = numNames[n] + " Hundred" + soFar;
	while(ret.at(0)==' ') ret = ret.substr(1);
	for(int i = 0; i < ret.size(); i++){
		if(ret.at(i) == ' ') ret.at(i) = '-';
	}
	return ret;
}


string Global::getAsWordth(int n){
	string ret = getAsWord(n);
	for(int i = 0; i < ret.size(); i++){
		if(ret.at(i) == '-') ret.at(i) = ' ';
	}
	string lastWord;
	istringstream lin(ret);
	while(!lin.eof()){
		lin >> lastWord;
	}
	if(lastWord == "One"){
		ret.pop_back();ret.pop_back();ret.pop_back();
		ret += "First";
	}
	else if(lastWord == "Two"){
		ret.pop_back();ret.pop_back();ret.pop_back();
		ret += "Second";
	}
	else if(lastWord == "Three"){
		ret.pop_back();ret.pop_back();ret.pop_back();
		ret += "ird";
	}
	else if(lastWord == "Five"){
		ret.pop_back();ret.pop_back();
		ret += "fth";
	}
	else if(lastWord == "Nine"){
		ret.pop_back();
		ret += "th";
	}
	else if(lastWord == "Twelve"){
		ret.pop_back();ret.pop_back();
		ret += "fth";
	}
	else if(lastWord.at(lastWord.size()-1) == 'y'){
		ret.pop_back();
		ret += "ieth";
	}
	else{
		ret += "th";
	}
	for(int i = 0; i < ret.size(); i++){
		if(ret.at(i) == ' ') ret.at(i) = '-';
	}
	return ret;
}





