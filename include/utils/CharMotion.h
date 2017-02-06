#ifndef CHARMOTION_H__
#define CHARMOTION_H__

enum class CharMotionType
{
	USER, STATIC, RANDOAREA, PATHREPEATER
};

class CharMotion
{
public:

	CharMotion():m_charmotion_type(CharMotionType::STATIC){}
	CharMotion(CharMotionType t):m_charmotion_type(t){}

private:

	CharMotionType m_charmotion_type;
	// xy coord range for area to wander
	// xy coords for path to follow, etc
};

#endif