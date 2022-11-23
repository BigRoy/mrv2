#pragma once

#include <FL/Fl_Box.H>
#include <FL/Fl.H>

namespace mrv
{
    class StatusBar : public Fl_Box
    {
        float seconds_;
        Fl_Color labelcolor_;
        Fl_Color color_;

        static void clean_cb( StatusBar* o )
            {
                o->copy_label( "" );
                o->redraw();
            }
        
    public:
        StatusBar( int X, int Y, int W, int H, const char* L = 0 ) :
            Fl_Box( X, Y, W, H, L )
            {
            }

        void timeout( float seconds )
            {
                seconds_ = seconds;
            }

        void restore_colors()
            {
                color( color_ );
                labelcolor( labelcolor_ );
                redraw();
            }

        void copy_label( const char* msg )
            {
                color_   = color();
                labelcolor_ = labelcolor();
                color( 0x000000ff );
                labelcolor( FL_WHITE );
                Fl_Box::copy_label( msg );
                redraw();
                Fl::add_timeout( seconds_, (Fl_Timeout_Handler) clean_cb,
                                 this );
            }
    };
}