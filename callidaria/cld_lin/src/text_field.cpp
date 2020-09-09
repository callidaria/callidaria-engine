#include "../fcn/text_field.h"

TextField::TextField(Font* fnt,Renderer2D* r2d,glm::vec2 pos,const char* bp,glm::vec2 bpos,float width,float height)
	: m_r2d(r2d),m_pos(pos),m_bpos(bpos),m_width(width),m_height(height)
{
	rinst = m_r2d->add(bpos,width,height,bp);
	txt = Text(fnt);
}
void TextField::render(Frame* frame,Camera2D* cam2d,glm::vec4 col)
{
	if (frame->event_active&&input_active) { // !!evil branching
		for (int i=0;i<285;i++) { // ??maybe not iterate through all ascii instances for the alphabet
			if (frame->kb.ka[i]) {
				char c = (char)93+i;
				m_pos.x += txt.add(c,m_pos);
				cnt += c;
				char_amount++;
			}
		} txt.load_wcam(cam2d);
	} // !!write input prefix for activating

	m_r2d->prepare();
	m_r2d->render_sprite(rinst,rinst+1);
	txt.prepare();
	txt.render(char_amount,col);
}
