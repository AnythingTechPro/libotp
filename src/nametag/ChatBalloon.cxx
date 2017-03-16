#include "ChatBalloon.h"
#include <transparencyAttrib.h>

static const LVecBase3f g_text_shift(.1, -.05, 1.1);
static const double g_text_shift_prop = .08;
static const float g_native_width = 10;
static const float g_min_width = 2.5;
static const float g_bubble_padding = .3;
static const float g_bubble_padding_prop = .05;
static const float g_button_scale = 6;
static const LVecBase3f g_button_shift(-.2, 0, .6);

TypeHandle ChatBalloon::_type_handle;

ChatBalloon::ChatBalloon(NodePath& model) : m_model(model) {
    
}

ChatBalloon::~ChatBalloon() {
    
}

NodePath ChatBalloon::generate(const std::wstring& text, PT(TextFont) font, LVecBase4* text_color, LVecBase4* balloon_color, float wordwrap, NodePath* button) {
    NodePath root = NodePath("root");
    NodePath balloon = m_model.copy_to(root);
    NodePath top = balloon.find("**/top");
    NodePath middle = balloon.find("**/middle");
    NodePath bottom = balloon.find("**/bottom");
    
    LVecBase4f bcolor;
    if (balloon_color == nullptr)
        bcolor = LVecBase4f(1);
        
    else
        bcolor = *balloon_color;
    
    balloon.set_color(bcolor);
    if (bcolor.get_w() < 1.0)
        balloon.set_transparency(TransparencyAttrib::M_alpha);
    
    LVecBase4f tcolor;
    if (text_color == nullptr)
        tcolor = LVecBase4f(1, 1, 1, 1);
        
    else
        tcolor = *text_color;
    
    PT(TextNode) tn = new TextNode("text");
    tn->set_font(font);
    tn->set_wordwrap(wordwrap);
    tn->set_wtext(text);
    tn->set_text_color(tcolor);
    
    float width = tn->get_width();
    float height = tn->get_height();
    
    NodePath text_np = root.attach_new_node(tn);
    text_np.set_pos(g_text_shift);
    text_np.set_x(text_np, g_text_shift_prop * width);
    text_np.set_z(height);
    
    if (button != nullptr)
    {
        NodePath button_np = button->copy_to(root);
        button_np.set_pos(text_np, width, 0, -height);
        button_np.set_pos(button_np, g_button_shift);
        button_np.set_scale(g_button_scale);
    }
    
    if (width < g_min_width)
    {
        width = g_min_width;
        text_np.set_x(text_np, width / 2.);
        tn->set_align(TextNode::A_center);
    }
    
    width *= g_bubble_padding_prop + 1;
    width += g_bubble_padding;
    balloon.set_sx(width / g_native_width);
    middle.set_sz(height);
    top.set_z(top, height - 1);
    
    return root;
}
