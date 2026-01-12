//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <memory>
//#include <vector>
//#include <fstream>
//
//int main() {
//
//
//	sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Test");
//	window.setFramerateLimit(60);
//	sf::RectangleShape rect({ 50,60 });
//	rect.setPosition({ 50,90 });
//	rect.setFillColor(sf::Color(100, 250, 50));
//	float y1 = 5.5f;
//	float x1 = 2.5f;
//	float y2 = 3.5f;
//	float x2 = 1.5f;
//	/*sf::Vector2f tr = rect.getGlobalBounds();*/
//	sf::Vector2f tr = rect.getPosition();
//
//	sf::Font Myfont;
//	if (!Myfont.openFromFile("font/MontserratAlternates-Black.ttf"))
//	{
//		std::cout << "Could not load font \n" << std::endl;
//		exit(-1);
//	}
//	sf::Text t1(Myfont);
//	t1.setString("Green");
//	t1.setCharacterSize({ 10 });
//
//
//	t1.setPosition(tr);
//
//
//
//	sf::CircleShape circ({ 50 });
//	circ.setPosition({ 100,300 });
//	circ.setFillColor(sf::Color(250, 20, 6));
//
//
//	sf::Text t2(Myfont);
//	t2.setString("Red");
//	t2.setCharacterSize({ 25 });
//	sf::Vector2f tr1 = circ.getPosition();
//	t2.setPosition(tr1);
//
//
//	while (window.isOpen())
//	{
//		while (const std::optional event = window.pollEvent())
//		{
//			if (event->is<sf::Event::Closed>())
//				window.close();
//		}
//		rect.setPosition(sf::Vector2f(rect.getPosition().x + x1, rect.getPosition().y + y1));
//		t1.setPosition(sf::Vector2f(rect.getPosition().x + x1, t1.getPosition().y + y1));
//
//		if (rect.getGlobalBounds().position.x <= 0 ||
//			rect.getGlobalBounds().position.x + rect.getSize().x >= 800)
//		{
//			x1 = -x1;
//		}
//		if (rect.getGlobalBounds().position.y <= 0 ||
//			rect.getGlobalBounds().position.y + rect.getSize().y >= 800)
//		{
//			y1 = -y1;
//		}
//
//		circ.setPosition(sf::Vector2f(circ.getPosition().x + x2, circ.getPosition().y + y2));
//		t2.setPosition(sf::Vector2f(circ.getPosition().x + x2, t2.getPosition().y + y2));
//
//		float r = circ.getRadius();
//
//		if (circ.getPosition().x - r <= 0 ||
//			circ.getPosition().x + r >= 800)
//		{
//			x2 = -x2;
//		}
//
//		if (circ.getPosition().y - r <= 0 ||
//			circ.getPosition().y + r >= 800)
//		{
//			y2 = -y2;
//		}
//
//
//
//
//
//
//
//
//		window.clear();
//		window.draw(circ);
//		window.draw(rect);
//		window.draw(t1);
//		window.draw(t2);
//		window.display();
//	}
//
//
//	return 0;
//}




#/*include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
int main()
{
	sf::RenderWindow window(sf:: VideoMode ({ 800,800 }), " Rect");
	std::vector <sf::RectangleShape > Rectangles;
	for (int x = 0; x < 25; x++)
	{
        for (int y = 0; y < 15; y++)
		{
			sf::RectangleShape rect(sf::Vector2f (15,15));
                rect.setPosition(sf::Vector2f(static_cast<float>(x * 20), static_cast<float>(y * 20)));
                rect.setFillColor(sf::Color(x * 10, y * 10, 0));
				Rectangles.push_back(rect);
				
		}

	}
	while (window.isOpen())
	{
		while (std::optional Event1 = window.pollEvent())
		{
			if (Event1->is<sf::Event::Closed>())
			{
				window.close();
			}
		}
		window.clear();
		for (auto& a : Rectangles)

		{
			window.draw(a);
		}
		window.display();

	}

	return 0;
}*/