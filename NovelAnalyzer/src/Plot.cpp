//
//  Plot.cpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#include "Plot.hpp"


void Plot::display(){
	RenderWindow plot(VideoMode(1600, 1200), title);
	plot.clear(background);
	
	
	RectangleShape rect;
	
	
	
	plot.display();
	while(plot.isOpen()){
		Event event;
		while(plot.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				plot.close();
				return;
			}
		}
	}
}


void Plot::plotWordByChapter(vector<set<string>> words){
	RenderWindow plot(VideoMode(1600, 1200), title);
	Image icon;
	if (icon.loadFromFile(resourcePath() + "icon.png")) {
		plot.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	plot.setFramerateLimit(30);
	
	vector<vector<int>> wordCounts;
	vector<int> chapterLengths;
	int wordCount = book->getWordcount();
	for(int i = 0; i < book->getNumChapters(); i++){
		//cout << "Chapter " << i << ": " << book->getChapterStart(i) << " to " << book->getChapterEnd(i) << endl;
		chapterLengths.push_back(book->getChapterLength(i));
		wordCounts.push_back(vector<int>());
		for(int j = 0; j < words.size(); j++){
			wordCounts.at(i).push_back(book->countWordInChapter(words.at(j),i));
			//cout << j << " in chapter " << i << ": " << book->countWordInChapter(words.at(j),i) << endl;
		}
	}
	
	while(colors.size() < words.size()){
		colors.push_back(Color(rand()%255,rand()%255,rand()%255));
	}
	
	RectangleShape rect;
	Font font;
	Text text;
	font.loadFromFile(resourcePath() + "sansation.ttf");
	text.setFont(font);
	
	IntRect bounds(50,50,1500,1100);
	int xdraw = bounds.left;
	for(int i = 0; i < chapterLengths.size(); i++){
		int width = (float)chapterLengths.at(i) / (float)wordCount * (float)bounds.width;
		//cout << width << endl;
		if(words.size() > 1){
			float total = 0;
			for(int j = 0; j < wordCounts.at(i).size(); j++){
				total += wordCounts.at(i).at(j);
			}
			int ydraw = bounds.top;
			for(int j = 0; j < wordCounts.at(i).size(); j++){
				float height = (float)wordCounts.at(i).at(j) / total * (float)bounds.height;
				if((int)height <= 0) continue;
				string m = "\"" + (*words.at(j).begin()) + "\"\n";
				m += "Chapter " + to_string(i+1) + "\n";
				m += to_string(wordCounts.at(i).at(j)) + " (" + to_string((int)((height / bounds.height)*100)) +"%)";
				zones.push_back(activeZone(IntRect(xdraw,ydraw,width,height),m,colors.at(j)));
				ydraw += height;
			}
		}
		else{
			
		}
		xdraw += width;
	}
	
	
	
	
	while(plot.isOpen()){
		Event event;
		while(plot.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				plot.close();
			}
		}
		
		plot.clear(background);
		
		for(int i = 0; i < zones.size(); i++){
			zones.at(i).fillArea(&plot, rect);
		}
		
		Vector2i mousepos = getMousePos(&plot,Vector2u(1600,1200));//Mouse::getPosition();
		for(int i = 0; i < zones.size(); i++){
			if(zones.at(i).isMouseOver(mousepos.x, mousepos.y)){
				zones.at(i).drawTip(&plot, text, mousepos.x, mousepos.y, 1600, 1200, true);
			}
		}
		
		plot.display();
	}
	zones.clear();
}



void Plot::makeColorPlot(){
	Data::loadColorData();
	vector<string> colorWords;
	vector<Color> colors;
	for(auto a: Data::colorData){
		colorWords.push_back(a.first);
		colors.push_back(a.second);
	}
	vector<ColorMarker> words;
	for(int i = 0; i < colorWords.size(); i++){
		vector<int>* places = book->getWordList(colorWords.at(i));
		if(places == nullptr) continue;
		for(int j = 0; j < places->size(); j++){
			words.push_back(ColorMarker(places->at(j), colors.at(i)));
		}
	}
	colorWords.clear();
	colors.clear();
	sort(words.begin(),words.end());
	
	vector<int> chapterLengths;
	int wordCount = book->getWordcount();
	for(int i = 0; i < book->getNumChapters(); i++){
		chapterLengths.push_back(book->getChapterLength(i));
	}
	
	IntRect bounds(50,50,1500,1100);
	Image image;
	image.create(bounds.width,bounds.height);
	
	vector<Color> chapterColors;
	vector<Color> c;
	vector<Color> temp;
	vector<Color> chapterColorsTemp;
	for(int i = 0; i < words.size(); i++){
		temp.push_back(words.at(i).color);
		c.push_back(Data::mergeColors(temp));
		if(words.at(i).wordNum >= book->getChapterEnd(chapterColors.size())){
			chapterColors.push_back(Data::mergeColors(chapterColorsTemp));
			chapterColorsTemp.clear();
		}
		chapterColorsTemp.push_back(words.at(i).color);
	}
	chapterColors.push_back(Data::mergeColors(chapterColorsTemp));
	chapterColorsTemp.clear();
	temp.clear();
	
	int wn = 0;
	int p = 0;
	Color lineColor;
	for(int i = 0; i < bounds.width; i++){
		p = (float)i / (float)bounds.width * (float)wordCount;
		for(int j = 0; j < words.size()-1; j++){
			if(words.at(j).wordNum > p){
				wn = j;
				break;
			}
		}
		lineColor = Data::colorInterp(c.at(wn), c.at(wn+1), (float)(p-words.at(wn).wordNum) / (float)(words.at(wn+1).wordNum-words.at(wn).wordNum));
		for(int j = 0; j < bounds.height; j++){
			image.setPixel(i, j, lineColor);
		}
	}
	
	int xp = bounds.left;
	for(int i = 0; i < chapterLengths.size(); i++){
		int width = (float)chapterLengths.at(i) / (float)wordCount * (float)bounds.width;
		zones.push_back(activeZone(IntRect(xp,bounds.top,width,bounds.height), book->getChapterName(i), chapterColors.at(i)));
		xp += width;
	}
	
	Texture texture;
	texture.loadFromImage(image);
	Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(bounds.left, bounds.top);
	RectangleShape rect;
	Font font;
	Text text;
	font.loadFromFile(resourcePath() + "sansation.ttf");
	text.setFont(font);
	RenderWindow plot(VideoMode(1600, 1200), title);
	Image icon;
	if (icon.loadFromFile(resourcePath() + "icon.png")) {
		plot.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	plot.setFramerateLimit(30);
	while(plot.isOpen()){
		Event event;
		while(plot.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				plot.close();
			}
		}
		plot.clear(background);
		
		plot.draw(sprite);
		
		
		Vector2i mousepos = getMousePos(&plot,Vector2u(1600,1200));//Mouse::getPosition();
		for(int i = 0; i < zones.size(); i++){
			if(zones.at(i).isMouseOver(mousepos.x, mousepos.y)){
				zones.at(i).fillArea(&plot, rect);
				zones.at(i).drawTip(&plot, text, mousepos.x, mousepos.y, 1600, 1200, true);
			}
		}
		plot.display();
	}
	zones.clear();
}








int min(int a, int b){
	if(a < b) return a;
	return b;
}
float absf(float a){
	if(a < 0) return a*-1;
	return a;
}


void Plot::makeSentimentPlot(set<string>& plotAgainst){
	Data::loadSentimentData();
	IntRect bounds(80,150,1600-80-50,1000);
	vector<int>* data = book->makeSentimentPlot(Data::sentimentData, 20000);
	
	int y0 = bounds.top + bounds.height/2;
	float max = book->getMaxAbsSentiment();
	//cout << max << " " << y0 << endl;
	
	float h = (float)bounds.width / (float)data->size();
	
	vector<int> chapterLengths = book->getChapterLengths();
	int wordCount = book->getWordcount();
	
	
	vector<float> wordAppearances = book->countWordRateInChapter(plotAgainst);
	vector<float> connotationSlope = book->getSentimentRateByChapter();
	
	string firstWord = "";
	if(plotAgainst.size()>0){
		firstWord = *plotAgainst.begin();
		firstWord.at(0) = toupper(firstWord.at(0));
	}
	
	float biggestDif = Global::max(connotationSlope);
	if(Global::min(connotationSlope)*-1.0 > biggestDif) biggestDif = Global::min(connotationSlope);
	float mostWord = Global::max(wordAppearances);
	if(book->getNumChapters()>1){
		for(int i = 0; i < book->getNumChapters(); i++){
			float xp = bounds.left + (float)book->getChapterStart(i) / (float)wordCount * (float)bounds.width;
			float xend = bounds.left + (float)book->getChapterEnd(i) / (float)wordCount * (float)bounds.width;
			string s = book->getChapterName(i) + "\n";
			s += "Positivity: " + to_string(connotationSlope.at(i)*100.0) + "%";
			if(plotAgainst.size()>0) s += "\nFrequency of " + firstWord + ": " + to_string(wordAppearances.at(i)*100.0) + "%";
			zones.push_back(activeZone(IntRect(xp,bounds.top,xend-xp+3,bounds.height), s));
			Color c = Color(0,200,0);
			if(connotationSlope.at(i) < 0) c = Color(200,0,0);
			float height = connotationSlope.at(i) / biggestDif * (float)(bounds.height / 4);
			zones.push_back(activeZone(IntRect(xp,y0-height,xend-xp,height), "", c));
			height = wordAppearances.at(i) / mostWord * (float)(bounds.height / 8.0);
			zones.push_back(activeZone(IntRect(xp,bounds.top+bounds.height-height,xend-xp,height), "", Color(250,250,0)));
		}
	}
	/*
	else{
		for(int i = 0; i < 20; i++){
			float xp = bounds.left + ((float)i/20.0) * (float)bounds.width;
			float xend = bounds.left + ((float)(i+1)/20.0) * (float)bounds.width;
			string s = "";
			s += "Positivity: " + to_string(connotationSlope.at(i)*100.0) + "%";
			//if(plotAgainst.size()>0) s += "\nFrequency of " + firstWord + ": " + to_string(wordAppearances.at(i)*100.0) + "%";
			zones.push_back(activeZone(IntRect(xp,bounds.top,xend-xp+3,bounds.height), s));
			Color c = Color(0,200,0);
			if(connotationSlope.at(i) < 0) c = Color(200,0,0);
			float height = connotationSlope.at(i) / biggestDif * (float)(bounds.height / 4);
			zones.push_back(activeZone(IntRect(xp,y0-height,xend-xp,height), "", c));
			height = wordAppearances.at(i) / mostWord * (float)(bounds.height / 8.0);
			zones.push_back(activeZone(IntRect(xp,bounds.top+bounds.height-height,xend-xp,height), "", Color(250,250,0)));
		}
	}
	 */
	
	float correlation = 0;
	if(plotAgainst.size() > 0) correlation = Data::findCorrelation(wordAppearances, connotationSlope);
	float slope = 0;
	if(plotAgainst.size() > 0) slope = Data::findBestFitSlope(wordAppearances, connotationSlope);
	
	bool prevMouseState = false;
	RectangleShape rect;
	Font font;
	Text text;
	font.loadFromFile(resourcePath() + "sansation.ttf");
	text.setFont(font);
	RenderWindow plot(VideoMode(1600, 1200), title);
	Image icon;
	if (icon.loadFromFile(resourcePath() + "icon.png")) {
		plot.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	plot.setFramerateLimit(30);
	while(plot.isOpen()){
		Event event;
		while(plot.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				plot.close();
			}
		}
		plot.clear(background);
		
		rect.setPosition(bounds.left, bounds.top);
		rect.setSize(Vector2f(bounds.width,bounds.height));
		rect.setFillColor(Color(50,50,50));
		rect.setOutlineThickness(3);
		rect.setOutlineColor(Color::White);
		plot.draw(rect);
		
		if(plotAgainst.size()==0)writeText(&plot, text, Vector2f(100,6), "Sentiment Plot", 64, Color::White, 3);
		else writeText(&plot, text, Vector2f(100,6), "Sentiment Plot vs Occurrence of Word \"" + firstWord + "\"", 64, Color::White, 3);
		writeText(&plot, text, Vector2f(100,80), book->getFullTitle(), 32, Color::White, 3);
		
		for(int i = 0; i < zones.size(); i++){
			zones.at(i).fillArea(&plot, rect);
		}
		
		Vertex line1[] = { Vertex(Vector2f(bounds.left,y0),Color::White), Vertex(Vector2f(bounds.left+bounds.width,y0),Color::White) };
		plot.draw(line1, 2, Lines);
		Vertex line2[] = { Vertex(Vector2f(bounds.left,bounds.top),Color::White), Vertex(Vector2f(bounds.left,bounds.top+bounds.height),Color::White) };
		plot.draw(line2, 2, Lines);
		for(int i = 0; i < book->getNumChapters(); i++){
			float xp = bounds.left + (float)book->getChapterEnd(i) / (float)wordCount * (float)bounds.width;
			Vertex line[] = { Vertex(Vector2f(xp,bounds.top),Color(100,100,100)), Vertex(Vector2f(xp,bounds.top+bounds.height),Color(100,100,100)) };
			plot.draw(line, 2, Lines);
		}
		
		if(plotAgainst.size() > 0) writeText(&plot, text, Vector2f(100,1150), "Correlation: " + to_string(correlation) + "\tSlope: " + to_string(slope), 32, Color::White, 3);
		
		Vector2f prevPoint(bounds.left,y0);
		for(int i = 0; i < data->size()-1; i++){
			Vector2f newPoint(prevPoint.x+h,y0-(float)data->at(i+1)/max*((float)bounds.height/2.0));
			for(int j = 0; j < 25; j++){
				Vertex line[] = { Vertex(Vector2f(prevPoint.x+j/5,prevPoint.y+j%5),Color::White), Vertex(Vector2f(newPoint.x+j/5,newPoint.y+j%5),Color::White) };
				plot.draw(line, 2, Lines);
			}
			prevPoint = newPoint;
		}
		
		Vector2i mousepos = getMousePos(&plot,Vector2u(1600,1200));
		if(mousepos.x >= bounds.left && mousepos.y >= bounds.top && mousepos.x <= bounds.left+bounds.width && mousepos.y <= bounds.top+bounds.height){
			Vertex line[] = { Vertex(Vector2f(mousepos.x,bounds.top),Color::White), Vertex(Vector2f(mousepos.x,bounds.top+bounds.height),Color::White) };
			plot.draw(line, 2, Lines);
			int ind = (float)(mousepos.x - bounds.left) / (float)(bounds.width) * (float)data->size();
			if(ind < 0) ind = 0; if(ind >= data->size()) ind = data->size()-1;
			int val = data->at(ind);
			float yp = y0 - ((float)val / max) * (float)(bounds.height / 2);
			Vertex line2[] = { Vertex(Vector2f(bounds.left,yp),Color::White), Vertex(Vector2f(bounds.left+bounds.width,yp),Color::White) };
			plot.draw(line2, 2, Lines);
			writeText(&plot, text, Vector2f(10,yp-20), to_string(val), 32, Color::White, 2);
		}
		
		for(int i = 0; i < zones.size(); i++){
			if(zones.at(i).isMouseOver(mousepos.x, mousepos.y)){
				zones.at(i).drawTip(&plot, text, mousepos.x, mousepos.y, 1600, 1200, false);
				break;
			}
		}
		
		rect.setPosition(1600-150, 1200-40);
		rect.setSize(Vector2f(100,35));
		rect.setFillColor(Color(0,120,180));
		rect.setOutlineThickness(1);
		rect.setOutlineColor(Color::White);
		if(mousepos.x > 1600-150 && mousepos.x < 1600-150+100 && mousepos.y > 1200-40 && mousepos.y < 1200-40+35){
			rect.setFillColor(Color(200,0,0));
			if(Mouse::isButtonPressed(Mouse::Left)){
				rect.setFillColor(Color(150,0,0));
				if(!prevMouseState){
					cout << "Sentiment Data" << endl;
					for(int i = 0; i < connotationSlope.size(); i++){
						if(plotAgainst.size() > 0) cout << wordAppearances.at(i) << "\t";
						cout << connotationSlope.at(i) << endl;
					}
					cout << endl;
				}
				prevMouseState = true;
			}
			else{
				prevMouseState = false;
			}
		}
		plot.draw(rect);
		writeText(&plot, text, Vector2f(1600-133, 1200-38), "PRINT", 24, Color::White, 0);
		
		plot.display();
	}
	zones.clear();
}














/*
void Plot::makeSentimentPlot(set<string>& plotAgainst){
	Data::loadSentimentData();
	IntRect bounds(80,150,1600-80-50,1000);
	vector<int>* data = book->makeSentimentPlot(Data::sentimentData, bounds.width/2);
	
	int y0 = bounds.top + bounds.height/2;
	float max = 1;
	for(int i = 0; i < data->size(); i++){
		//cout << data->at(i) << endl;
		if(data->at(i) > max) max = data->at(i);
		if(0-data->at(i) > max) max = 0-data->at(i);
	}
	//cout << max << " " << y0 << endl;
	
	float h = (float)bounds.width / (float)data->size();
	
	vector<int> chapterLengths;
	int wordCount = book->getWordcount();
	for(int i = 0; i < book->getNumChapters(); i++){
		chapterLengths.push_back(book->getChapterLength(i));
	}
	
	
	vector<float> wordAppearances;
	vector<float> connotationSlope;
	
	
	int beforeScore = 0;
	float biggestDif = 0;
	vector<int> chapterScores;
	vector<int> countsInChapter;
	float mostWord = 0;
	for(int i = 0; i < book->getNumChapters(); i++){
		countsInChapter.push_back(book->countWordInChapter(plotAgainst,i));
		float wordSlope = (float)countsInChapter.at(i) / (float)chapterLengths.at(i);
		wordAppearances.push_back(wordSlope);
		if(wordSlope > mostWord) mostWord = wordSlope;
		float xp = bounds.left + (float)book->getChapterStart(i) / (float)wordCount * (float)bounds.width;
		float xend = bounds.left + (float)book->getChapterEnd(i) / (float)wordCount * (float)bounds.width;
		int ind = (float)book->getChapterEnd(i) / (float)wordCount * (float)data->size();
		if(ind>=data->size()) ind = data->size()-1;
		int scoreDif = data->at(ind) - beforeScore;
		chapterScores.push_back(scoreDif);
		if(absf((float)scoreDif/(float)chapterLengths.at(i)) > biggestDif) biggestDif = absf((float)scoreDif/(float)chapterLengths.at(i));
		string s = book->getChapterName(i) + "\n";
		float sl = ((float)scoreDif/(float)chapterLengths.at(i)*100.0);
		connotationSlope.push_back(sl);
		s += "Slope: " + to_string(sl) + "%\n";
		s += "(" + to_string(beforeScore) + " to " + to_string(data->at(ind)) + ")";
		zones.push_back(activeZone(IntRect(xp,bounds.top,xend-xp+3,bounds.height), s));
		beforeScore = data->at(ind);
	}
	//cout << mostWord << endl;
	for(int i = 0; i < book->getNumChapters(); i++){
		Color c = Color(0,200,0);
		if(chapterScores.at(i) < 0) c = Color(200,0,0);
		float height = ((float)chapterScores.at(i)/(float)chapterLengths.at(i)) / (float)biggestDif * (float)(bounds.height / 4);
		float xp = bounds.left + (float)book->getChapterStart(i) / (float)wordCount * (float)bounds.width;
		float xend = bounds.left + (float)book->getChapterEnd(i) / (float)wordCount * (float)bounds.width;
		zones.push_back(activeZone(IntRect(xp,y0-height,xend-xp,height), "", c));
		height = ((float)countsInChapter.at(i)/(float)chapterLengths.at(i)) / mostWord * (float)(bounds.height / 8.0);
		zones.push_back(activeZone(IntRect(xp,bounds.top+bounds.height-height,xend-xp,height), "", Color(250,250,0)));
	}
	
	float correlation = 0;
	if(plotAgainst.size() > 0) correlation = Data::findCorrelation(wordAppearances, connotationSlope);
	
	RectangleShape rect;
	Font font;
	Text text;
	font.loadFromFile(resourcePath() + "sansation.ttf");
	text.setFont(font);
	RenderWindow plot(VideoMode(1600, 1200), title);
	Image icon;
	if (icon.loadFromFile(resourcePath() + "icon.png")) {
		plot.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	plot.setFramerateLimit(30);
	while(plot.isOpen()){
		Event event;
		while(plot.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				plot.close();
			}
		}
		plot.clear(background);
		
		rect.setPosition(bounds.left, bounds.top);
		rect.setSize(Vector2f(bounds.width,bounds.height));
		rect.setFillColor(Color(50,50,50));
		rect.setOutlineThickness(3);
		rect.setOutlineColor(Color::White);
		plot.draw(rect);
		
		writeText(&plot, text, Vector2f(100,6), "Sentiment Plot", 64, Color::White, 3);
		writeText(&plot, text, Vector2f(100,80), book->getFullTitle(), 32, Color::White, 3);
		
		for(int i = 0; i < zones.size(); i++){
			zones.at(i).fillArea(&plot, rect);
		}
		
		Vertex line1[] = { Vertex(Vector2f(bounds.left,y0),Color::White), Vertex(Vector2f(bounds.left+bounds.width,y0),Color::White) };
		plot.draw(line1, 2, Lines);
		Vertex line2[] = { Vertex(Vector2f(bounds.left,bounds.top),Color::White), Vertex(Vector2f(bounds.left,bounds.top+bounds.height),Color::White) };
		plot.draw(line2, 2, Lines);
		for(int i = 0; i < book->getNumChapters(); i++){
			float xp = bounds.left + (float)book->getChapterEnd(i) / (float)wordCount * (float)bounds.width;
			Vertex line[] = { Vertex(Vector2f(xp,bounds.top),Color(100,100,100)), Vertex(Vector2f(xp,bounds.top+bounds.height),Color(100,100,100)) };
			plot.draw(line, 2, Lines);
		}
		
		if(plotAgainst.size() > 0) writeText(&plot, text, Vector2f(100,1150), "Correlation: " + to_string(correlation), 32, Color::White, 3);
		
		Vector2f prevPoint(bounds.left,y0);
		for(int i = 0; i < data->size()-1; i++){
			Vector2f newPoint(prevPoint.x+h,y0-(float)data->at(i+1)/max*((float)bounds.height/2.0));
			for(int j = 0; j < 25; j++){
				Vertex line[] = { Vertex(Vector2f(prevPoint.x+j/5,prevPoint.y+j%5),Color::White), Vertex(Vector2f(newPoint.x+j/5,newPoint.y+j%5),Color::White) };
				plot.draw(line, 2, Lines);
			}
			prevPoint = newPoint;
		}
		
		Vector2i mousepos = getMousePos(&plot,Vector2u(1600,1200));
		if(mousepos.x >= bounds.left && mousepos.y >= bounds.top && mousepos.x <= bounds.left+bounds.width && mousepos.y <= bounds.top+bounds.height){
			Vertex line[] = { Vertex(Vector2f(mousepos.x,bounds.top),Color::White), Vertex(Vector2f(mousepos.x,bounds.top+bounds.height),Color::White) };
			plot.draw(line, 2, Lines);
			int ind = (float)(mousepos.x - bounds.left) / (float)(bounds.width) * (float)data->size();
			if(ind < 0) ind = 0; if(ind >= data->size()) ind = data->size()-1;
			int val = data->at(ind);
			float yp = y0 - ((float)val / max) * (float)(bounds.height / 2);
			Vertex line2[] = { Vertex(Vector2f(bounds.left,yp),Color::White), Vertex(Vector2f(bounds.left+bounds.width,yp),Color::White) };
			plot.draw(line2, 2, Lines);
			writeText(&plot, text, Vector2f(10,yp-20), to_string(val), 32, Color::White, 2);
		}
		
		for(int i = 0; i < zones.size(); i++){
			if(zones.at(i).isMouseOver(mousepos.x, mousepos.y)){
				zones.at(i).drawTip(&plot, text, mousepos.x, mousepos.y, 1600, 1200, false);
				break;
			}
		}
		plot.display();
	}
	zones.clear();
}
*/




