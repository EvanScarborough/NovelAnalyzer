//
//  WordCount.hpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#ifndef Book_hpp
#define Book_hpp

#include <stdio.h>
#include "Global.hpp"
#include "Data.hpp"

enum ChapterNameType{
	FIRSTCHAPTER, CHAPTER1
};


class Book{
public:
	Book(string file){
		fileName = file;
		title = file;
		readMeta();
		read();
	}
	void setTitle(string t){title=t;}
	string getTitle(){return title;}
	void setAuthor(string t){author=t;}
	string getAuthor(){return author;}
	int getNumChapters(){int r = chapterMark.size(); if(r==0) return 1; return r;}
	int getChapterLength(int cnum){int r = getChapterEnd(cnum)-getChapterStart(cnum); if(r==0)return 1; return r;}
	vector<int> getChapterLengths(){vector<int> ret;for(int i=0;i<getNumChapters();i++)ret.push_back(getChapterLength(i));return ret;}
	int getWordcount(){return wordCount;}
	
	int delimChapters(set<string> delim);
	int delimChapters(int length);
	
	
	vector<WordCount> getMostCommon(int num, set<string>& ignore);
	vector<WordCount> getMostCommon(int num){
		set<string> ignore;
		return getMostCommon(num, ignore);
	}
	
	int countWord(string w){ if(dictionary.find(w)==dictionary.end())return 0; return dictionary.at(w).size(); }
	float countWordRate(string w){return (float)countWord(w) / (float)getWordcount(); }
	int countWord(set<string>& w){ int r = 0; for(string a:w)r+=countWord(a); return r; }
	float countWordRate(set<string>& w){return (float)countWord(w) / (float)getWordcount(); }
	int countWord(string w, int min, int max);
	float countWordRate(string w, int min, int max){return (float)countWord(w,min,max) / (float)(max - min); }
	int countWord(set<string>& w, int min, int max){ int r = 0; for(string a:w)r+=countWord(a,min,max); return r; }
	float countWordRate(set<string>& w, int min, int max){return (float)countWord(w,min,max) / (float)(max-min); }
	int countWordInChapter(string w, int cnum);
	float countWordRateInChapter(string w, int cnum){ return (float)countWordInChapter(w, cnum) / (float)getChapterLength(cnum); }
	int countWordInChapter(set<string>& w, int cnum){ int r = 0; for(string a:w)r+=countWordInChapter(a,cnum); return r; }
	float countWordRateInChapter(set<string>& w, int cnum){ return (float)countWordInChapter(w, cnum) / (float)getChapterLength(cnum); }
	vector<int> countWordInChapter(string w){vector<int> ret;for(int i=0;i<getNumChapters();i++)ret.push_back(countWordInChapter(w,i));return ret;}
	vector<float> countWordRateInChapter(string w){vector<float> ret;for(int i=0;i<getNumChapters();i++)ret.push_back(countWordRateInChapter(w,i));return ret;}
	vector<int> countWordInChapter(set<string>& w){vector<int> ret;for(int i=0;i<getNumChapters();i++)ret.push_back(countWordInChapter(w,i));return ret;}
	vector<float> countWordRateInChapter(set<string>& w){vector<float> ret;for(int i=0;i<getNumChapters();i++)ret.push_back(countWordRateInChapter(w,i));return ret;}
	vector<WordCount> countWords(vector<string>& words);
	vector<WordCount> countWords(vector<set<string>>& words);
	vector<float> countWordRateByDelim(string w,int h){vector<float> ret;for(int i=0;i<wordCount;i+=h)ret.push_back(countWordRate(w,i,i+h));return ret;}
	
	int getChapterStart(int n){
		if(n < 0 || (n == 0 && chapterMark.size() == 0)) return 0;
		if(n >= chapterMark.size()) return wordCount;
		return chapterMark.at(n);
	}
	int getChapterEnd(int n){
		if(n < 0) return 0;
		if(n+1 >= chapterMark.size()) return wordCount;
		return chapterMark.at(n+1);
	}
	
	bool has(string word){ return dictionary.find(word) != dictionary.end(); }
	vector<int>* getWordList(string w){if(!has(w))return nullptr;return &dictionary.at(w);}
	
	vector<int>* makeSentimentPlot(map<string,int>& data, int dataPoints);
	
	string getFullTitle(){
		string ret = "\"" + title + "\"";
		if(author.size() > 0) ret += " by " + author;
		if(year != -1) ret += " (" + to_string(year) + ")";
		return ret;
	}
	string getChapterName(int n);
	
	vector<WordFrequency> inverseDocumentFrequency(set<Book*>& inverseFiles,bool omitPunctuation);
	vector<WordFrequency> inverseDocumentFrequency(set<Book*>& inverseFiles){
		return inverseDocumentFrequency(inverseFiles,true);
	}
	vector<WordFrequency> inverseDocumentFrequency(Book& inverse){
		set<Book*> inv;
		inv.emplace(&inverse);
		return inverseDocumentFrequency(inv);
	}
	
	void resetSentiment(){ sentimentPlot.clear(); sentimentData = nullptr; }
	
	int getSentimentScoreAt(int word);
	int getSentimentBetween(int min, int max);
	float getSentimentRateBetween(int min, int max);
	int getSentimentByChapter(int chapter);
	float getSentimentRateByChapter(int chapter);
	vector<int> getSentimentByChapter();
	vector<float> getSentimentRateByChapter();
	vector<float> getSentimentRateByDelim(int h);
	int getMaxSentiment(){int ret=0; for(int i=0;i<sentimentPlot.size();i++)if(sentimentPlot.at(i)>ret)ret=sentimentPlot.at(i); return ret;}
	int getMinSentiment(){int ret=0; for(int i=0;i<sentimentPlot.size();i++)if(sentimentPlot.at(i)<ret)ret=sentimentPlot.at(i); return ret;}
	int getMaxAbsSentiment(){int ret=getMaxSentiment();int min=getMinSentiment();if(min*-1>ret)return min*-1;return ret;}
	
	vector<WordFrequency> findMostSentimentalCorr(int minCount);
	vector<WordFrequency> findMostSentimentalCorr(int minCount, int h);
	vector<WordFrequency> findMostSentimentalCorrNeutral(int minCount, int h);
	vector<WordFrequency> findMostSentimental(int minCount, float minCorrelation);
	
	string getWordAt(int n);
	string getNextWord(string w);
	string markov(int numWords, int chain);
	
private:
	string fileName;
	string author = "";
	string title = "???";
	int year = -1;
	string chapterName = "Chapter";
	ChapterNameType chapterNameType = CHAPTER1;
	
	map<string,vector<int>> dictionary;
	
	vector<int> chapterMark;
	set<string> chapterDelim;
	int wordCount = 0;
	int numChapters = 0;
	
	vector<int> sentimentPlot;
	map<string,int>* sentimentData = nullptr;
	
	
	
	void read();
	void readMeta();
	void submitWord(string word, int wordnum){
		if(word.size() == 0) return;
		//cout << word  << " " << wordnum << endl;
		if(dictionary.find(word) == dictionary.end()){
			vector<int> pages;
			pages.push_back(wordnum);
			dictionary.emplace(word,pages);
		}
		else{
			dictionary.at(word).push_back(wordnum);
		}
	}
};














#endif /* Book_hpp */
