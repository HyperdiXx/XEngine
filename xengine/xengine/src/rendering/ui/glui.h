#pragma once

#ifndef GLUIH
#define GLUIH

#include "ui.h"

#include <vector>

#include <math/vec4f.h>
#include <math/vec3f.h>

namespace XEngine
{
    class GLui : public UserInterface
    {
    public:
        GLui() {};
        
        void init(int theme);
        void setUIScene5(vec3f &pos, vec4f &color, unsigned int texture, vec4f &colo1, vec4f &clor2, vec4f color3, bool &pressed);

        void new_frame();
        void update(vec4f& spritecol);
        void shutdown();

        void top_bar();
        void post_update();

        void set_dark_theme();
        void set_light_theme();
        void set_classic_theme();
    private:
        std::vector<int> editLayer;
        
        enum Style
        {
            WHITE,
            DARK
        };
};
}


#endif // !GLUIH


