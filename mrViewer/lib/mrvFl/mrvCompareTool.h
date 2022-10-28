#pragma once

#include <vector>

#include <FL/Fl_Button.H>
#include <FL/Fl_RGB_Image.H>

#include <tlCore/Time.h>

#include "mrvToolWidget.h"

class ViewerUI;

namespace mrv
{
    using namespace tl;
    
    class CompareTool : public ToolWidget
    {
    public:
        CompareTool( ViewerUI* ui );
        ~CompareTool();

        void clear_controls();
        void add_controls() override;

        
        void redraw();
        
        void refresh();
        void compareThumbnail( const int64_t id,
                               const std::vector< std::pair<otime::RationalTime,
                               Fl_RGB_Image*> >& thumbnails,
                               Fl_Button* w);
    private:
        struct Private;
        std::unique_ptr<Private> _r;
    };


} // namespace mrv