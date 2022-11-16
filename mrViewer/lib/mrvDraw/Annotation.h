#pragma once

#include <memory>
#include <vector>

#include <mrvDraw/Shape.h>


namespace tl
{
    namespace draw
    {

        class Annotation
        {
        public:
            Annotation( const int64_t frame );
            ~Annotation();

            int64_t frame() const;
            
            void push_back(const std::shared_ptr< Shape >&);

            const std::vector< std::shared_ptr< Shape > >& shapes() const;
            
            std::shared_ptr< Shape > lastShape() const;
            
            void undo();
            
            void redo();

        protected:
            TLRENDER_PRIVATE();
        };

        
        typedef std::vector< std::shared_ptr<Annotation> > AnnotationList;

    }
    
}