
#pragma once

#include ".hpp/drawobj.hpp"

#include "blend2d/image.h"

// Requires that init is called and the size is set for drawing
class BlendObj : public DrawObj
{
ASSIGN_CONSTRUCTOR(BlendObj);

protected:
    BLImage image;

    pos image_size;

public:
    virtual void save(Saver* ar) const override
    {
        Object::save(ar);
        
        ar->save_complex(size);
        ar->save_complex(image_size);
    }

    virtual void load(Loader* ar) override
    {
        Object::load(ar);
        
        size = ar->load_complex<pos>();
        image_size = ar->load_complex<pos>();
    }
    
    void update_image();

    // Updates image (calling update_image) to new_image and sets it
    void set_image(BLImage new_image);

    BLImage get_image();

    void set_size(pos new_size);
};