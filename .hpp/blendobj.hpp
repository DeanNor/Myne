
#pragma once

#include ".hpp/drawobj.hpp"

#include ".hpp/sprite.hpp"
#include "blend.h"

// Requires that set_depth or target is called and the size is set for drawing
class BlendObj : public DrawObj
{
ASSIGN_CONSTRUCTOR(BlendObj);

protected:
    BLImage image;

    pos image_size;

public:
    BlendObj()
    {
        texture = new sprite();
    }

    virtual void save(Saver* ar) const override
    {
        Object::save(ar);
        
        ar->save_complex(image_size);
    }

    virtual void load(Loader* ar) override
    {
        Object::load(ar);
        
        image_size = ar->load_complex<pos>();
    }
    
    void update_image();

    void update_image_size();

    // Updates image (calling update_image) to new_image and sets it
    void set_image(BLImage new_image);

    BLImage get_image();
};