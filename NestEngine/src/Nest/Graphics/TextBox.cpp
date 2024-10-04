#include "TextBox.h"

Nest::TextBox::TextBox() :
	m_pos(0.f),
	m_fontPoint(0.f)
{}

Nest::TextBox::TextBox(chcl::Vector2<float> pos, const std::string &text, float pnt, Ref<Font> font) :
	m_font(font),
	m_pos(pos),
	m_fontPoint(pnt)
{
	setText(text);
	updateTransform();
}

void Nest::TextBox::setPosition(chcl::Vector2<float> pos)
{
	m_pos = pos;
	updateTransform();
}

void Nest::TextBox::setText(const std::string &text)
{
	m_textVA = m_font->generateVertexArray(text);
	m_textIB = m_font->generateIndexBuffer(text);
}

void Nest::TextBox::setPnt(float pnt)
{
	m_fontPoint = pnt;
	updateTransform();
}

void Nest::TextBox::updateTransform()
{
	float fUnitScale = m_fontPoint / m_font->getUnitsPerEm();
	m_transform = chcl::Mat4::Translation3D(m_pos.x, m_pos.y, 0.f) * chcl::Mat4::Scale3D(fUnitScale, fUnitScale, 0.f);
}