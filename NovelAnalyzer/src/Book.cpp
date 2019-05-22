//
//  WordCount.cpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#include "Book.hpp"


void Book::readMeta(){
	ifstream fin(resourcePath() + "books/meta/" + fileName + ".txt");
	if(fin.fail()){
		return;
	}
	string line;
	string arg;
	while(!fin.eof()){
		getline(fin,line);
		istringstream lin(line);
		lin >> arg;
		Global::format(arg, SIMPLE);
		if(arg == "title" || arg == "name"){
			string n = fileName;
			getline(lin,n);
			while(n.at(0) == ' ') n = n.substr(1);
			title = n;
		}
		else if(arg == "author" || arg == "by"){
			string a = "";
			getline(lin,a);
			while(a.at(0) == ' ') a = a.substr(1);
			author = a;
		}
		else if(arg == "year"){
			lin >> year;
		}
		else if(arg == "chapterdelim" || arg == "chapter"){
			bool f = true;
			while(!lin.eof()){
				string c = "";
				lin >> c;
				if(c.size() > 0){
					if(f){
						chapterName = c;
						f = false;
					}
					chapterDelim.emplace(c);
				}
			}
		}
		else if(arg == "numbering" || arg == "chapternum" || arg == "chapternumbering"){
			string n = "";
			lin >> n;
			Global::format(n,SIMPLE);
			if(n == "firstchapter"){
				chapterNameType = FIRSTCHAPTER;
			}
			else if(n == "chapter1"){
				chapterNameType = CHAPTER1;
			}
		}
		else if(arg == "chaptername"){
			string a = "";
			getline(lin,a);
			while(a.at(0) == ' ') a = a.substr(1);
			chapterName = a;
		}
	}
}




void Book::read(){
	ifstream fin(resourcePath() + "books/" + fileName + ".txt");
	if(fin.fail()){
		cout << "Failed to load book: " << fileName << ".txt!" << endl;
		cout << "Make sure it is in the folder called 'books'" << endl;
		return;
	}
	char c;
	bool readingPunctuation = false;
	string word = "";
	dictionary.clear();
	int n = 0;
	
	while(!fin.eof()){
		c = fin.get();
		if(isalpha(c) || isnumber(c) || ((c=='-' || c=='\'') && word.size()>0)){ // reading a word (or number)
			if(readingPunctuation && word.size() > 0){
				if(chapterDelim.find(word) != chapterDelim.end()){
					chapterMark.push_back(n);
				}
				submitWord(word,n); // submitted punctuation
				word = "";
			}
			word.push_back(c);
			readingPunctuation = false;
		}
		else if(c == '.' || c == '!' || c == '?' || c == ','){ // we don't care about these.
			if(!readingPunctuation && word.size() > 0){
				if(chapterDelim.find(word) != chapterDelim.end()){
					chapterMark.push_back(n);
				}
				Global::format(word,SIMPLE);
				submitWord(word,n); // submitted real word
				if(Global::hasAlpha(word)) n++;
				word = "";
			}
			word.push_back(c);
			readingPunctuation = true;
		}
		else{ // we don't care
			if(chapterDelim.find(word) != chapterDelim.end()){
				chapterMark.push_back(n);
			}
			if(!readingPunctuation) Global::format(word,SIMPLE);
			submitWord(word,n); // submitted something
			if(!readingPunctuation && Global::hasAlpha(word)) n++;
			word = "";
			readingPunctuation = false;
		}
	}
	//cout << chapterMark.size() << endl;
	wordCount = n;
}



int Book::delimChapters(set<string> delim){
	ifstream fin(resourcePath() + "books/" + fileName + ".txt");
	if(fin.fail()){
		cout << "Failed to load book: " << fileName << ".txt!" << endl;
		cout << "Make sure it is in the folder called 'books'" << endl;
		return;
	}
	char c;
	bool readingPunctuation = false;
	chapterDelim = delim;
	chapterMark.clear();
	string word = "";
	int n = 0;
	
	while(!fin.eof()){
		c = fin.get();
		if(isalpha(c) || isnumber(c) || ((c=='-' || c=='\'') && word.size()>0)){ // reading a word (or number)
			if(readingPunctuation && word.size() > 0){
				if(delim.find(word) != delim.end()){
					chapterMark.push_back(n);
				}
				word = "";
			}
			word.push_back(c);
			readingPunctuation = false;
		}
		else if(c == '.' || c == '!' || c == '?' || c == ','){ // we don't care about these.
			if(!readingPunctuation && word.size() > 0){
				if(delim.find(word) != delim.end()){
					chapterMark.push_back(n);
				}
				if(Global::hasAlpha(word))n++;
				word = "";
			}
			word.push_back(c);
			readingPunctuation = true;
		}
		else{ // we don't care
			if(delim.find(word) != delim.end()){
				chapterMark.push_back(n);
			}
			if(!readingPunctuation) Global::format(word,SIMPLE);
			if(!readingPunctuation && Global::hasAlpha(word)) n++;
			word = "";
			readingPunctuation = false;
		}
	}
	numChapters = chapterMark.size();
	return numChapters;
}


int Book::delimChapters(int length){
	chapterMark.clear();
	chapterMark.push_back(0);
	for(int i = 0; i < wordCount; i+=length){
		chapterMark.push_back(i);
	}
	numChapters = chapterMark.size();
	return numChapters;
}




vector<WordCount> Book::countWords(vector<string>& words){
	vector<WordCount> ret;
	for(int i = 0; i < words.size(); i++){
		ret.push_back(WordCount(words.at(i),countWord(words.at(i))));
	}
	sort(ret.begin(),ret.end(),greater<WordCount>());
	return ret;
}
vector<WordCount> Book::countWords(vector<set<string>>& words){
	vector<WordCount> ret;
	for(int i = 0; i < words.size(); i++){
		int c = 0;
		string name = "";
		for(string a:words.at(i)){
			c += countWord(a);
			if(name.size()==0) name = a;
		}
		ret.push_back(WordCount(name, c));
	}
	sort(ret.begin(),ret.end(),greater<WordCount>());
	return ret;
}

int Book::countWord(string w, int min, int max){
	if(dictionary.find(w) == dictionary.end()) return 0;
	vector<int>* v = &dictionary.at(w);
	int count = 0;
	for(int i: *v){
		if(i >= min && i < max) count++;
	}
	return count;
}

int Book::countWordInChapter(string w, int cnum){
	return countWord(w,getChapterStart(cnum),getChapterEnd(cnum));
}





string Book::getChapterName(int n){
	if(chapterNameType == CHAPTER1){
		return chapterName + " " + to_string(n+1);
	}
	if(chapterNameType == FIRSTCHAPTER){
		return Global::getAsWordth(n+1) + " " + chapterName;
	}
}







vector<int>* Book::makeSentimentPlot(map<string,int>& data, int dataPoints){
	if(sentimentData == &data && sentimentPlot.size() > 0){
		return &sentimentPlot;
	}
	sentimentData = &data;
	
	sentimentPlot.push_back(0);
	int pointDistance = wordCount / dataPoints;
	ifstream fin(resourcePath() + "books/" + fileName + ".txt");
	if(fin.fail()){
		cout << "Failed to load book: " << fileName << ".txt!" << endl;
		cout << "Make sure it is in the folder called 'books'" << endl;
		return;
	}
	char c;
	bool readingPunctuation = false;
	string word = "";
	int n = 0;
	
	//cout << data.size() << endl;
	
	while(!fin.eof()){
		c = fin.get();
		if(isalpha(c) || isnumber(c) || ((c=='-' || c=='\'') && word.size()>0)){ // reading a word (or number)
			if(readingPunctuation && word.size() > 0){
				if(data.find(word) != data.end()){
					//cout << word << endl;
					sentimentPlot.at(sentimentPlot.size()-1) += data.at(word);
				}
				word = "";
			}
			word.push_back(c);
			readingPunctuation = false;
		}
		else if(c == '.' || c == '!' || c == '?' || c == ','){ // we don't care about these.
			if(!readingPunctuation && word.size() > 0){
				Global::format(word,SIMPLE);
				if(data.find(word) != data.end()){
					//cout << word << endl;
					sentimentPlot.at(sentimentPlot.size()-1) += data.at(word);
				}
				if(Global::hasAlpha(word)) n++;
				word = "";
			}
			word.push_back(c);
			readingPunctuation = true;
		}
		else{ // we don't care
			if(!readingPunctuation) Global::format(word,SIMPLE);
			if(data.find(word) != data.end()){
				//cout << word << endl;
				sentimentPlot.at(sentimentPlot.size()-1) += data.at(word);
			}
			if(!readingPunctuation && Global::hasAlpha(word)) n++;
			word = "";
			readingPunctuation = false;
		}
		
		if(n >= pointDistance){
			sentimentPlot.push_back(sentimentPlot.at(sentimentPlot.size()-1));
			n = 0;
		}
	}
	return &sentimentPlot;
}



int Book::getSentimentBetween(int min, int max){
	if(sentimentPlot.size()==0) makeSentimentPlot(Data::sentimentData, 1500);
	return getSentimentScoreAt(max) - getSentimentScoreAt(min);
}
float Book::getSentimentRateBetween(int min, int max){
	return (float)getSentimentBetween(min, max) / (float)(max-min);
}
int Book::getSentimentByChapter(int chapter){
	if(chapter <= 0) return 0;
	vector<int> c = getSentimentByChapter();
	if(chapter >= c.size()) return 0;
	return c.at(chapter);
}
float Book::getSentimentRateByChapter(int chapter){
	return (float)getSentimentByChapter(chapter) / (float)getChapterLength(chapter);
}
vector<int> Book::getSentimentByChapter(){
	vector<int> ret;
	for(int i = 0; i < getNumChapters(); i++){
		ret.push_back(getSentimentBetween(getChapterStart(i), getChapterEnd(i)));
	}
	return ret;
}
vector<float> Book::getSentimentRateByChapter(){
	vector<int> sbc = getSentimentByChapter();
	vector<float> ret;
	for(int i = 0; i < sbc.size(); i++){
		ret.push_back((float)sbc.at(i) / (float)getChapterLength(i));
	}
	return ret;
}
vector<float> Book::getSentimentRateByDelim(int h){
	vector<float> ret;
	for(int i = 0; i < wordCount; i+=h){
		int e = i+h;
		if(e > wordCount) e = wordCount;
		ret.push_back(getSentimentBetween(i,e));
	}
	return ret;
}
int Book::getSentimentScoreAt(int word){
	if(sentimentPlot.size()==0) return 0;
	int i1 = (float)word / (float)getWordcount() * (float)sentimentPlot.size();
	if(i1 < 0) i1 = 0;
	if(i1 >= sentimentPlot.size()) i1 = sentimentPlot.size()-1;
	return sentimentPlot.at(i1);
}



vector<WordFrequency> Book::inverseDocumentFrequency(set<Book*>& inverseFiles,bool omitPunctuation){
	map<string,float> wordFrequencies;
	for(auto a: dictionary){
		string word = a.first;
		if(!Global::hasAlpha(word) && omitPunctuation) continue;
		wordFrequencies.emplace(a.first,((float)a.second.size())/(float)wordCount);
	}
	for(Book* b: inverseFiles){
		for(auto a: b->dictionary){
			if(wordFrequencies.find(a.first) == wordFrequencies.end()) continue;
			wordFrequencies.at(a.first) = wordFrequencies.at(a.first) - ((float)a.second.size())/(float)b->getWordcount();
		}
	}
	vector<WordFrequency> ret;
	for(auto a: wordFrequencies){
		ret.push_back(WordFrequency(a.first,a.second));
	}
	sort(ret.begin(),ret.end(),greater<WordFrequency>());
	for(int i=0; i<ret.size(); i++){
		cout << i+1 << ": " << ret.at(i).word << " " << ret.at(i).freq*100.0 << "%" << endl;
	}
	return ret;
}



vector<WordFrequency> Book::findMostSentimentalCorr(int minCount){
	vector<WordFrequency> ret;
	
	vector<float> sentimentScore = getSentimentRateByChapter();
	for(auto a: dictionary){
		if(a.second.size() < minCount) continue;
		vector<float> wordFreq = countWordRateInChapter(a.first);
		float correlation = Data::findCorrelation(wordFreq, sentimentScore);
		//cout << wordFreq.size() << " " << sentimentScore.size() << " " << correlation << endl;
		ret.push_back(WordFrequency(a.first, correlation));
	}
	sort(ret.begin(),ret.end(),greater<WordFrequency>());
	for(int i=0; i<ret.size(); i++){
		cout << i+1 << ": " << ret.at(i).word << " " << ret.at(i).freq*100.0 << "%" << endl;
	}
	
	return ret;
}


vector<WordFrequency> Book::findMostSentimental(int minCount, float minCorrelation){
	vector<WordFrequency> ret;
	
	vector<float> sentimentScore = getSentimentRateByChapter();
	for(auto a: dictionary){
		if(a.second.size() < minCount) continue;
		vector<float> wordFreq = countWordRateInChapter(a.first);
		float correlation = Data::findCorrelation(wordFreq, sentimentScore);
		if(Global::abs(correlation) < minCorrelation) continue;
		float slope = Data::findBestFitSlope(wordFreq, sentimentScore);
		//cout << wordFreq.size() << " " << sentimentScore.size() << " " << correlation << endl;
		ret.push_back(WordFrequency(a.first, slope));
	}
	sort(ret.begin(),ret.end(),greater<WordFrequency>());
	for(int i=0; i<ret.size(); i++){
		cout << i+1 << ": " << ret.at(i).word << " " << ret.at(i).freq << endl;
	}
	
	return ret;
}


vector<WordFrequency> Book::findMostSentimentalCorr(int minCount, int h){
	vector<WordFrequency> ret;
	
	vector<float> sentimentScore = getSentimentRateByDelim(h);
	for(auto a: dictionary){
		if(a.second.size() < minCount) continue;
		vector<float> wordFreq = countWordRateByDelim(a.first,h);
		float correlation = Data::findCorrelation(wordFreq, sentimentScore);
		//cout << wordFreq.size() << " " << sentimentScore.size() << " " << correlation << endl;
		ret.push_back(WordFrequency(a.first, correlation));
	}
	sort(ret.begin(),ret.end(),greater<WordFrequency>());
	for(int i=0; i<ret.size(); i++){
		cout << i+1 << ": " << ret.at(i).word << " " << ret.at(i).freq*100.0 << "%" << endl;
	}
	
	return ret;
}

vector<WordFrequency> Book::findMostSentimentalCorrNeutral(int minCount, int h){
	vector<WordFrequency> ret;
	
	vector<float> sentimentScore = getSentimentRateByDelim(h);
	for(auto a: dictionary){
		if(a.second.size() < minCount) continue;
		if(Data::sentimentData.find(a.first) != Data::sentimentData.end()) continue;
		vector<float> wordFreq = countWordRateByDelim(a.first,h);
		float correlation = Data::findCorrelation(wordFreq, sentimentScore);
		//cout << wordFreq.size() << " " << sentimentScore.size() << " " << correlation << endl;
		ret.push_back(WordFrequency(a.first, correlation));
	}
	sort(ret.begin(),ret.end(),greater<WordFrequency>());
	for(int i=0; i<ret.size(); i++){
		cout << i+1 << ": " << ret.at(i).word << " " << ret.at(i).freq*100.0 << "%" << endl;
	}
	
	return ret;
}


string Book::getWordAt(int n){
	for(auto a: dictionary){
		for(int i = 0; i < a.second.size(); i++){
			if(a.second.at(i) == n) return a.first;
		}
	}
	return "";
}

string Book::getNextWord(string w){
	if(!has(w)) return "";
	return getWordAt(dictionary.at(w).at(Global::rnum(dictionary.at(w).size())) + 1);
}

string Book::markov(int numWords, int chain){
	srand(time(NULL));
	for(int i = 0; i < 3; i++) Global::rnum(100);
	
	map<string,vector<char>> markovChain;
	
	ifstream fin(resourcePath() + "books/" + fileName + ".txt");
	if(fin.fail()){
		cout << "Failed to load book: " << fileName << ".txt!" << endl;
		cout << "Make sure it is in the folder called 'books'" << endl;
		return;
	}
	string mon = "";
	while(!fin.eof() && mon.size() < chain){
		mon.push_back(fin.get());
	}
	string wordOn = mon;
	while(!fin.eof()){
		char next = fin.get();
		if(!isalnum(next) && next != '.' && next != '!' && next != '?' && next != ',' && next != '-' && next != ' ' && next != '\n') continue;
		if(next == '\n') next = ' ';
		if(markovChain.find(mon) == markovChain.end()){
			markovChain.emplace(mon,vector<char>());
		}
		markovChain.at(mon).push_back(next);
		mon.erase(mon.begin());
		mon.push_back(next);
	}
	string ret = wordOn;
	for(int i = 0; i < numWords; i++){
		if(markovChain.find(wordOn) == markovChain.end()){cout << "not found" << endl; break;}
		char next = markovChain.at(wordOn).at(Global::rnum(markovChain.at(wordOn).size()));
		ret.push_back(next);
		wordOn.erase(wordOn.begin());
		wordOn.push_back(next);
	}
	
	cout << ret << endl;
	
	/*
	string wordOn = getNextWord(".");
	while(wordOn.size() == 0) wordOn = getWordAt(Global::rnum(getWordcount()));
	
	cout << wordOn << " ";
	for(int i = 0; i < numWords; i++){
		wordOn = getNextWord(wordOn);
		cout << wordOn << " ";
	}
	*/
	
	return ret;
}












