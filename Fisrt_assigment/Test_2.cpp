//#include <iostream>
//#include <SFML/Graphics.hpp>
//#include <fstream>
//#include <vector>
//#include <memory>
//
//struct ShapeData
//{
//	std:: string   type;
//	std:: string   name;
//	sf :: Vector2f pos;
//	sf :: Vector2f size;
//	sf :: Vector2f speed;
//	sf :: Color    color;
//	      float    size1 = 0;
//		  float    size2 = 0;
//};
//
//int main()
//{
//	//File Loading 
//	std::ifstream file("config_data.txt");
//	if (!file)
//	{
//		std::cerr << "File Not Found" << std::endl;
//		exit(-1);
//	}
//
//	//For Window and Font
//	std :: string token ;
//	unsigned int winW = 800, winH = 800; 
//	std :: string fontpath ;
//	unsigned int fontsize = 15;
//	sf  :: Color fontcolor = sf :: Color :: White ;
//	
//	//A vector To Hold all Of teh Shapes
//
//	std::vector <ShapeData> A_Shape;
//
//	//Function for Bounding Box
//	auto Center_text = [](sf::Text& text, sf::Shape& shape)
//		{
//			sf::FloatRect TextBoundry = text.getLocalBounds();
//			//Setting the origin
//			text.setOrigin(sf::Vector2f(TextBoundry.position.x + TextBoundry.size.x / 2.0f,
//				TextBoundry.position.y + TextBoundry.size.y / 2.0f));
//
//			//Now for the Position in the shape
//			sf::FloatRect ShapeBoundry = shape.getGlobalBounds();
//			shape.setPosition(sf::Vector2f(ShapeBoundry.position.x + ShapeBoundry.size.x / 2.0f
//			, ShapeBoundry.position.y + ShapeBoundry.size.y / 2.0f));
//
//		};
//	
//
//	//Reading From File Using The Token Variabe
//	while (file >> token)
//	{
//		if (token == "window")
//		{
//			file >> winW >> winH;
//		}
//		else if (token == "Font")
//		{
//			std::uint8_t r, g, b;
//			file >> fontpath >> fontsize >> r >> g >> b;
//			fontcolor = sf::Color(r, g, b);
//		}
//		else if (token == "Circle")
//		{
//			ShapeData sdCirc;
//			sdCirc.type = token;
//			std::uint8_t r, g, b;
//			file >> sdCirc.name >> sdCirc.pos.x >> sdCirc.pos.y
//				>> sdCirc.speed.x >> sdCirc.speed.y
//				>> r >> g >> b
//				>> sdCirc.size1; // For Radius Bec Circ
//			sdCirc.color = sf::Color({ r,g,b });
//			A_Shape.push_back(sdCirc);
//		}
//		else if (token == "Rectangle")
//		{
//			ShapeData sdrect;
//			sdrect.type = token;
//			std::uint8_t r, g, b;
//			file >> sdrect.name >> sdrect.pos.x >> sdrect.pos.y
//				>> sdrect.speed.x >> sdrect.speed.y
//				>> r >> g >> b
//				>> sdrect.size1 >> sdrect.size2;
//			sdrect.color = sf::Color({ r,g,b });
//			A_Shape.push_back(sdrect);
//		}
//		
//
//	}
//    
//
//	//Creating Window 
//
//	sf::RenderWindow window(sf::VideoMode({ winW,winH }), token);
//	window.setFramerateLimit(90);
//
//
//	// font  loading 
//	sf::Font Myfont;
//	if (!Myfont.openFromFile(fontpath))
//	{
//		std::cerr << "Font Not Found" << std::endl;
//		exit(-1);
//	}
//
//	//Main Game Loop 
//	while (window.isOpen())
//	{
//		while (const std::optional event = window.pollEvent())
//		{
//		 window.close();
//		}
//
//		window.clear();
//		window.display();
//
//	}
//	return 0;
//}