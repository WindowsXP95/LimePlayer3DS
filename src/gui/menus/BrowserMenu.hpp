#ifndef __LP_BROWSERMENU_HPP__
#define __LP_BROWSERMENU_HPP__
#include <cmath>
#include <memory>

#include "menu.hpp"
#include "explorer.hpp"

class BrowserMenu : public Menu
{
	public:
		BrowserMenu();
		~BrowserMenu();
		void drawTop() const override;
		void drawBottom() const override;
		void update(touchPosition* touch) override;
		void fblist(int rows, int startpoint, float size) const;
	private:
		std::unique_ptr<Explorer> expInst;
};

#endif