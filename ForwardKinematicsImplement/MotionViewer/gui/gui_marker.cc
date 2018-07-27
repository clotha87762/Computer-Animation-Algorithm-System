#include "gui_marker.h"

namespace gui {

Marker::Marker()
    :target_pos_(Vector3d_t::Zero()),
    init_pos_(Vector3d_t::Zero()),
    color_id_(),
    is_select_(FALSE)
{
    unsigned char color[] = {0, 0, 0};
    color_id_.assign(color, color + 3);
}

Marker::~Marker()
{
}

Vector3d_t Marker::init_pos() const
{
    return init_pos_;
}

Vector3d_t Marker::target_pos() const
{
    return this->target_pos_;
}

std::vector<unsigned char> Marker::color_id() const
{
    return this->color_id_;
}

bool Marker::is_select() const
{
    return this->is_select_;
}


void Marker::set_color_id(const unsigned char r, const unsigned char g, const unsigned char b)
{
    unsigned char color[] = {r, g, b};
    color_id_.assign(color, color + 3);
}

void Marker::set_init_pos(const Vector3d_t &init_pos)
{
    this->init_pos_ = init_pos;
}

void Marker::set_target_pos(const Vector3d_t &target_pos)
{
    this->target_pos_ = target_pos;
}

void Marker::ResetPos()
{
    this->target_pos_ = this->init_pos_;
}

void Marker::Select()
{
    this->is_select_ = TRUE;
    this->set_color_id(color_id_[0], color_id_[2], color_id_[1]);
}

void Marker::Release()
{
    this->is_select_ = FALSE;
    this->set_color_id(color_id_[0], color_id_[2], color_id_[1]);
}

void Marker::Move(const Vector3d_t &shift_vector)
{
    target_pos_ += shift_vector;
}

} // namespace gui {
