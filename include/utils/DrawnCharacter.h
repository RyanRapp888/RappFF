#ifndef DRAWNCHARACTER_H__
#define DRAWNCHARACTER_H__

#include "DrawableObj.h"
#include "Texture.h"
#include "RGB.h"

class DrawnCharacter : public DrawableObj
{
public:

	DrawnCharacter();
	virtual ~DrawnCharacter() {}
	void SetTileType(TileType tt);
	//we may want to eventually do reference counting on the texture
	void SetColor(RGB col);
	void Render();

private:

	Texture *m_texture;
	TileType m_tiletype;
	RGB m_color;
};

#endif
