
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include "Global.hpp"
#include "Book.hpp"
#include "Plot.hpp"

int main(int, char const**) {
	
	Book book("messages");
	Plot plot(book);
	
	book.delimChapters(500);
	//set<string> words;
	//words.emplace("i");
	//words.emplace("pink");
	//words.emplace("i");
	
	
	plot.makeSentimentPlot();
	
	
	/*
	set<Book*> inverses;
	Book tom("TomSawyer");
	inverses.emplace(&tom);
	Book alice("AliceInWonderland");
	inverses.emplace(&alice);
	Book mice("We");
	inverses.emplace(&mice);
	Book jane("JaneEyre");
	inverses.emplace(&jane);
	Book frank("Frankenstein");
	inverses.emplace(&frank);
	//book.inverseDocumentFrequency(inverses,false);
	 //*/
	
	//*
	//Data::loadSentimentData();
	//book.makeSentimentPlot(Data::sentimentData,20000);
	//book.findMostSentimentalCorr(10);
	//book.findMostSentimentalCorrNeutral(10,500);
	
	//book.findMostSentimentalCorrNeutral(5,40);
	//*/
	
	//book.markov(500,8);
	
    return EXIT_SUCCESS;
}













