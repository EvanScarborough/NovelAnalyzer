//
//  Plot.hpp
//  NovelAnalyzer
//
//  Created by Evan Scarborough on 1/25/18.
//  Copyright © 2018 VectoPlasm. All rights reserved.
//

#ifndef Plot_hpp
#define Plot_hpp

#include <stdio.h>
#include "Global.hpp"
#include "Book.hpp"
#include "Data.hpp"


enum GraphType{ LINE, BAR, STACKBAR };

class activeZone{
public:
	IntRect zone;
	string message;
	Color color;
	bool isMouseOver(float xmouse, float ymouse){
		return xmouse > zone.left && xmouse < zone.left+zone.width && ymouse > zone.top && ymouse < zone.top+zone.height;
	}
	activeZone(IntRect z, string m){
		zone = z;
		message = m;
		color = Color(0,0,0,0);
	}
	activeZone(IntRect z, string m, Color c){
		zone = z;
		message = m;
		color = c;
	}
	void fillArea(RenderWindow* window, RectangleShape& rect){
		rect.setPosition(zone.left, zone.top);
		rect.setSize(Vector2f(zone.width,zone.height));
		rect.setOutlineThickness(0);
		rect.setFillColor(color);
		window->draw(rect);
	}
	void drawTip(RenderWindow* window, Text& text, float xmouse, float ymouse, int width, int height, bool outline){
		if(message.size() == 0) return;
		text.setColor(Color::White);
		text.setString(message);
		text.setCharacterSize(32);
		RectangleShape background;
		background.setPosition(zone.left, zone.top);
		background.setFillColor(Color(0,0,0,0));
		background.setOutlineThickness(6);
		background.setOutlineColor(Color::White);
		background.setSize(Vector2f(zone.width,zone.height));
		if(outline) window->draw(background);
		background.setSize(Vector2f(text.getLocalBounds().width+BUFFER,text.getLocalBounds().height+BUFFER));
		float xp = xmouse + OFFSET;
		float yp = ymouse + OFFSET;
		if(xmouse > width/2) xp = xmouse - background.getSize().x - OFFSET;
		if(ymouse > height/2) yp = ymouse - background.getSize().y - OFFSET;
		background.setPosition(xp+SHADOWDIST,yp+SHADOWDIST);
		background.setFillColor(Color(0,0,0,100));
		background.setOutlineThickness(0);
		window->draw(background);
		background.setPosition(xp,yp);
		background.setFillColor(Color(0,0,0,150));
		background.setOutlineThickness(6);
		window->draw(background);
		text.setPosition(xp+BUFFER/2,yp+BUFFER/5);
		window->draw(text);
	}
	static const int SHADOWDIST = 15;
	static const int OFFSET = 25;
	static const int BUFFER = 20;
};

class Plot{
public:
	string title = "Word Plot";
	string xlabel = "";
	string ylabel = "";
	bool grid = false;
	vector<Color> colors;
	
	Book* book;
	
	Color background = Color(120,120,120);
	
	GraphType type = LINE;
	
	vector<activeZone> zones;
	
	Plot(Book& b) {
		book = &b;
	}
	
	void plotWordByChapter(vector<set<string>> words);
	
	void makeColorPlot();
	
	void makeSentimentPlot(set<string>& plotAgainst);
	void makeSentimentPlot(){
		set<string> p;
		makeSentimentPlot(p);
	}
	void makeSentimentPlot(string s){
		set<string> p;
		p.emplace(s);
		makeSentimentPlot(p);
	}
	
	void display();
	
	Vector2i getMousePos(RenderWindow* w, Vector2u originalSize){
		Vector2i m = Mouse::getPosition(*w);
		return Vector2i((float)m.x/(float)w->getSize().x*(float)originalSize.x,(float)m.y/(float)w->getSize().y*(float)originalSize.y);
	}
	
	
	
	
	
	
	void writeText(RenderWindow* w, Text& text, Vector2f pos, string str, int charSize, Color color, int shadow){
		text.setCharacterSize(charSize);
		text.setString(str);
		if(shadow){
			text.setColor(Color::Black);
			text.setPosition(Vector2f(pos.x + shadow, pos.y + shadow));
			w->draw(text);
		}
		text.setPosition(pos);
		text.setColor(color);
		w->draw(text);
	}
};








#endif /* Plot_hpp */
