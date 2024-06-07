#ifndef INLINES_H_
#define INLINES_H_

#define Max(x,y) ((x > y) ? x : y)

#define RadToDeg(rad) (rad * 180.0f / M_PI)
#define DegToRad(deg) (deg * M_PI / 180.0f)

#define RectIsEqual(rect1,rect2) ((rect1).x == (rect2).x && (rect1).y == (rect2).y && (rect1).w == (rect2).w && (rect1).h == (rect2).h)

#define BlendIsEqual(blend1,blend2) (\
    (blend1).srcRGB == (blend2).srcRGB &&\
    (blend1).dstRGB == (blend2).dstRGB &&\
    (blend1).srcAlpha == (blend2).srcAlpha &&\
    (blend1).dstAlpha == (blend2).dstAlpha &&\
    (blend1).modeRGB == (blend2).modeRGB &&\
    (blend1).modeAlpha == (blend2).modeAlpha\
)

#define SetUnTextureVertex(gap_buffer,x,y,color)                                                                         \
do{                                                                                                                      \
    if(UnTextureAttributeSize > GapSize(gap_buffer,float)){                                                              \
        ExpandGapBuffer(gap_buffer,float,UnTextureAttributeSize - GapSize(gap_buffer,float) + (gap_buffer)->expand_size);\
    }                                                                                                                    \
    ((float*)(gap_buffer)->gap_start)[0] = x;                                                                            \
    ((float*)(gap_buffer)->gap_start)[1] = y;                                                                            \
    ((float*)(gap_buffer)->gap_start)[2] = (color).r / 255.0f;                                                           \
    ((float*)(gap_buffer)->gap_start)[3] = (color).g / 255.0f;                                                           \
    ((float*)(gap_buffer)->gap_start)[4] = (color).b / 255.0f;                                                           \
    ((float*)(gap_buffer)->gap_start)[5] = (color).a / 255.0f;                                                           \
    (gap_buffer)->gap_start = (float*)(gap_buffer)->gap_start + UnTextureAttributeSize;                                  \
}while(0)

#define SetTextureVertex(gap_buffer,x,y,s,t,color)                                                                     \
do{                                                                                                                    \
    if(TextureAttributeSize > GapSize(gap_buffer,float)){                                                              \
        ExpandGapBuffer(gap_buffer,float,TextureAttributeSize - GapSize(gap_buffer,float) + (gap_buffer)->expand_size);\
    }                                                                                                                  \
    ((float*)(gap_buffer)->gap_start)[0] = x;                                                                          \
    ((float*)(gap_buffer)->gap_start)[1] = y;                                                                          \
    ((float*)(gap_buffer)->gap_start)[2] = s;                                                                          \
    ((float*)(gap_buffer)->gap_start)[3] = t;                                                                          \
    ((float*)(gap_buffer)->gap_start)[4] = (color).r / 255.0f;                                                         \
    ((float*)(gap_buffer)->gap_start)[5] = (color).g / 255.0f;                                                         \
    ((float*)(gap_buffer)->gap_start)[6] = (color).b / 255.0f;                                                         \
    ((float*)(gap_buffer)->gap_start)[7] = (color).a / 255.0f;                                                         \
    (gap_buffer)->gap_start = (float*)(gap_buffer)->gap_start + TextureAttributeSize;                                  \
}while(0)

#define ChangeBlendMode(renderer,blend_mode)                                                                     \
do{                                                                                                              \
    if(!BlendIsEqual(blend_mode,(renderer)->last_blend_mode)){                                                   \
        WG_FlushBlitBuffer(renderer);                                                                               \
        (renderer)->last_blend_mode = blend_mode;                                                                \
        glBlendFuncSeparate((blend_mode).srcRGB,(blend_mode).dstRGB,(blend_mode).srcAlpha,(blend_mode).dstAlpha);\
        glBlendEquationSeparate((blend_mode).modeRGB,(blend_mode).modeAlpha);                                    \
    }                                                                                                            \
}while(0)

#define ChangeBlending(renderer,blending)         \
do{                                               \
    if((renderer)->last_use_blending != blending){\
        WG_FlushBlitBuffer(renderer);                \
        if(blending) glEnable(GL_BLEND);          \
        else         glDisable(GL_BLEND);         \
        (renderer)->last_use_blending = blending; \
    }                                             \
}while(0)

#define ChangeViewport(renderer,viewport)                                                                                       \
do{                                                                                                                             \
    if(!RectIsEqual(viewport,(renderer)->last_viewport)){                                                                       \
        (renderer)->last_viewport = viewport;                                                                                   \
        if((renderer)->current_target != NULL){                                                                                 \
            glViewport((viewport).x,(renderer)->current_target->height - (viewport).h - (viewport).y,(viewport).w,(viewport).h);\
        }                                                                                                                       \
        else{                                                                                                                   \
            glViewport((viewport).x,(renderer)->window_height - (viewport).h - (viewport).y,(viewport).w,(viewport).h);         \
        }                                                                                                                       \
    }                                                                                                                           \
}while(0)

#define SetClipRect(renderer)                                                                                                        \
do{                                                                                                                                  \
    if((renderer)->current_view->use_cliprect){                                                                                      \
        glEnable(GL_SCISSOR_TEST);                                                                                                   \
        int y = (renderer)->current_view->viewport.h - ((renderer)->current_view->cliprect.y + (renderer)->current_view->cliprect.h);\
        glScissor((renderer)->current_view->cliprect.x,y,(renderer)->current_view->cliprect.w,(renderer)->current_view->cliprect.h); \
    }                                                                                                                                \
}while(0)

#define UnSetClipRect(renderer)                \
do{                                            \
    if((renderer)->current_view->use_cliprect){\
        glDisable(GL_SCISSOR_TEST);            \
    }                                          \
}while(0)

#define FlipBitmap(bitmap,w,h)                                   \
do{                                                              \
    uint8_t t;                                                   \
    int hw = roundf(h * 0.5f);                                   \
    for(int y=0; y<hw; ++y){                                     \
        for(int x=0; x<w; ++x){                                  \
            t = (bitmap)[y * w + x];                             \
            (bitmap)[y * w + x] = (bitmap)[(h - y - 1) * w + x]; \
            (bitmap)[(h - y - 1) * w + x] = t;                   \
        }                                                        \
    }                                                            \
}while(0)

#define MakePixelsRGBA(bitmap,w,h,pixels)                             \
do{                                                                   \
    for(int y=0; y<h; ++y){                                           \
        for(int x=0; x<w; ++x){                                       \
            (pixels)[y * (w * 4) + (x * 4) + 0] = 255;                \
            (pixels)[y * (w * 4) + (x * 4) + 1] = 255;                \
            (pixels)[y * (w * 4) + (x * 4) + 2] = 255;                \
            (pixels)[y * (w * 4) + (x * 4) + 3] = (bitmap)[y * w + x];\
        }                                                             \
    }                                                                 \
}while(0)


#define _DrawFilledRectangle(renderer,rect,color)                                                           \
do{                                                                                                        \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                                           \
                                                                                                           \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;                  \
    float x1,y1,x2,y2;                                                                                     \
                                                                                                           \
    if(rect != NULL){                                                                                      \
        x1 = (rect)->x;                                                                                    \
        y1 = (rect)->y;                                                                                    \
        x2 = (rect)->x + (rect)->w;                                                                        \
        y2 = (rect)->y + (rect)->h;                                                                        \
    }                                                                                                      \
    else{                                                                                                  \
        x1 = 0.0f;                                                                                         \
        y1 = 0.0f;                                                                                         \
        x2 = (renderer)->current_view->viewport.w;                                                         \
        y2 = (renderer)->current_view->viewport.h;                                                         \
    }                                                                                                      \
                                                                                                           \
    SetUnTextureVertex((renderer)->blit_buffer,x1,y1,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x2,y1,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x2,y2,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x1,y2,color);                                               \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);                                 \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                 \
}while(0)

#define _DrawFilledRectangles(renderer,rects,count,color)                            \
do{                                                                                 \
    if(!rects || !count) return;                                                    \
                                                                                    \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                    \
                                                                                    \
    uint32_t start_index;                                                           \
    float x1,y1,x2,y2;                                                              \
                                                                                    \
    for(uint32_t i = 0; i < count; ++i){                                            \
                                                                                    \
        start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                    \
        x1 = (rects)[i].x;                                                          \
        y1 = (rects)[i].y;                                                          \
        x2 = (rects)[i].x + (rects)[i].w;                                           \
        y2 = (rects)[i].y + (rects)[i].h;                                           \
                                                                                    \
        SetUnTextureVertex((renderer)->blit_buffer,x1,y1,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x2,y1,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x2,y2,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x1,y2,color);                    \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);      \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);      \
    }                                                                               \
}while(0)

#define _DrawRectangle(renderer,rect,color)                                                                 \
do{                                                                                                        \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                               \
                                                                                                           \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;                  \
    float x1,y1,x2,y2;                                                                                     \
                                                                                                           \
    if(rect != NULL){                                                                                      \
        x1 = (rect)->x;                                                                                    \
        y1 = (rect)->y;                                                                                    \
        x2 = (rect)->x + (rect)->w;                                                                        \
        y2 = (rect)->y + (rect)->h;                                                                        \
    }                                                                                                      \
    else{                                                                                                  \
        x1 = 0.0f;                                                                                         \
        y1 = 0.0f;                                                                                         \
        x2 = (renderer)->current_view->viewport.w;                                                         \
        y2 = (renderer)->current_view->viewport.h;                                                         \
    }                                                                                                      \
                                                                                                           \
    SetUnTextureVertex((renderer)->blit_buffer,x1,y1,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x2,y1,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x2,y2,color);                                               \
    SetUnTextureVertex((renderer)->blit_buffer,x1,y2,color);                                               \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                                 \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                 \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);                                 \
                                                                                                           \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);                                 \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                 \
}while(0)

#define _DrawRectangles(renderer,rects,count,color)                                  \
do{                                                                                 \
    if(!rects || !count) return;                                                    \
                                                                                    \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                        \
                                                                                    \
    uint32_t start_index;                                                           \
    float x1,y1,x2,y2;                                                              \
                                                                                    \
    for(uint32_t i = 0; i < count; ++i){                                            \
                                                                                    \
        start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                    \
        x1 = (rects)[i].x;                                                          \
        y1 = (rects)[i].y;                                                          \
        x2 = (rects)[i].x + (rects)[i].w;                                           \
        y2 = (rects)[i].y + (rects)[i].h;                                           \
                                                                                    \
        SetUnTextureVertex((renderer)->blit_buffer,x1,y1,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x2,y1,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x2,y2,color);                    \
        SetUnTextureVertex((renderer)->blit_buffer,x1,y2,color);                    \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);      \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);      \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);      \
                                                                                    \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);      \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);      \
    }                                                                               \
}while(0)

#define _DrawFilledTriangle(renderer,vertex1,vertex2,vertex3,color)                             \
do{                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                               \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex1).x,(vertex1).y,color);                 \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex2).x,(vertex2).y,color);                 \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex3).x,(vertex3).y,color);                 \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                     \
}while(0)

#define _DrawTriangle(renderer,vertex1,vertex2,vertex3,color)                                   \
do{                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                   \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex1).x,(vertex1).y,color);                 \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex2).x,(vertex2).y,color);                 \
    SetUnTextureVertex((renderer)->blit_buffer,(vertex3).x,(vertex3).y,color);                 \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                     \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                     \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                     \
}while(0)

#define _DrawFilledCircle(renderer,center,radius,color)                                                      \
do{                                                                                                         \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                                            \
                                                                                                            \
    float tempx = 0.0f;                                                                                     \
    float dt = CircleSegmentAngleFactor / sqrtf(radius);                                                    \
    int number_of_segments = 2.0f * M_PI / dt;                                                              \
    int start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;                  \
    int index = start_index;                                                                                \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x,(center).y,color);                                \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                          \
                                                                                                            \
    float cos = cosf(dt);                                                                                   \
    float sin = sinf(dt);                                                                                   \
                                                                                                            \
    float dx = 1.0f;                                                                                        \
    float dy = 0.0f;                                                                                        \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                          \
                                                                                                            \
    tempx = dx * cos - dy * sin;                                                                            \
    dy = dx * sin + dy * cos;                                                                               \
    dx = tempx;                                                                                             \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                            \
                                                                                                            \
    for(int i=2; i<number_of_segments; ++i){                                                                \
        tempx = dx * cos - dy * sin;                                                                        \
        dy = dx * sin + dy * cos;                                                                           \
        dx = tempx;                                                                                         \
                                                                                                            \
        GapBufferInsertValue(renderer->blit_index,uint32_t,start_index);                                    \
        GapBufferInsertValue(renderer->blit_index,uint32_t,index++);                                        \
                                                                                                            \
        SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);\
        GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                        \
    }                                                                                                       \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                                      \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index + 1);                                  \
}while(0)

#define _DrawCircle(renderer,center,radius,color)                                                            \
do{                                                                                                         \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                                \
                                                                                                            \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;                   \
    uint32_t index = start_index;                                                                           \
    float dt = CircleSegmentAngleFactor / sqrtf(radius);                                                    \
    int number_of_segments = 2.0f * M_PI / dt;                                                              \
                                                                                                            \
    float tempx = 0.0f;                                                                                     \
    float cos = cosf(dt);                                                                                   \
    float sin = sinf(dt);                                                                                   \
    float dx = 1.0f;                                                                                        \
    float dy = 0.0f;                                                                                        \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
                                                                                                            \
    for(int i=2; i<number_of_segments; ++i){                                                                \
        tempx = dx * cos - dy * sin;                                                                        \
        dy = dx * sin + dy * cos;                                                                           \
        dx = tempx;                                                                                         \
                                                                                                            \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                      \
                                                                                                            \
        SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);\
        GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                        \
    }                                                                                                       \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                                      \
}while(0)

#define _DrawFilledArc(renderer,center,radius,startAngle,endAngle,color)                                     \
do{                                                                                                         \
    if(endAngle - startAngle >= 360.0f){                                                                    \
        _DrawFilledCircle(renderer,center,radius,color);                                                     \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                                            \
                                                                                                            \
    float dx,dy,tempx;                                                                                      \
                                                                                                            \
    int start_index =  ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;                 \
    int index = start_index;                                                                                \
                                                                                                            \
    float dt = ((endAngle - startAngle) / 360.0f) * (CircleSegmentAngleFactor/sqrtf(radius));               \
    int numSegments = DegToRad(fabs(endAngle - startAngle)) / dt;                                           \
                                                                                                            \
    float cos = cosf(dt);                                                                                   \
    float sin = sinf(dt);                                                                                   \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x,(center).y,color);                                \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                          \
                                                                                                            \
    dx = cosf(DegToRad(startAngle));                                                                        \
    dy = sinf(DegToRad(startAngle));                                                                        \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                          \
                                                                                                            \
    tempx = dx * cos - dy * sin;                                                                            \
    dy = dx * sin + dy * cos;                                                                               \
    dx = tempx;                                                                                             \
	                                                                                                        \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                            \
                                                                                                            \
	for (int i = 2; i < numSegments; i++){                                                                  \
                                                                                                            \
        tempx = cos * dx - sin * dy;                                                                        \
        dy = sin * dx + cos * dy;                                                                           \
        dx = tempx;                                                                                         \
                                                                                                            \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                                  \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                      \
                                                                                                            \
        SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);\
        GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                        \
	}                                                                                                       \
                                                                                                            \
    dx = cosf(DegToRad(endAngle));                                                                          \
    dy = sinf(DegToRad(endAngle));                                                                          \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                                      \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index++);                                          \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,index);                                            \
}while(0)

#define _DrawArc(renderer,center,radius,startAngle,endAngle,color)                                           \
do{                                                                                                         \
    if(endAngle - startAngle >= 360.0f){                                                                    \
        _DrawCircle(renderer,center,radius,color);                                                           \
        return;                                                                                             \
    }                                                                                                       \
                                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                                \
                                                                                                            \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;             \
    uint32_t current_index = start_index;                                                                   \
    float dt = ((endAngle - startAngle) / 360.0f) * (CircleSegmentAngleFactor / sqrtf(radius));             \
    int number_of_segments = DegToRad(fabs(endAngle - startAngle)) / dt;                                    \
                                                                                                            \
    float tempx = 0.0f;                                                                                     \
    float cos = cosf(dt);                                                                                   \
    float sin = sinf(dt);                                                                                   \
    float dx = cosf(DegToRad(startAngle));                                                                  \
    float dy = sinf(DegToRad(startAngle));                                                                  \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
                                                                                                            \
    for(int i = 0; i<number_of_segments - 2; ++i){                                                          \
        tempx = dx * cos - dy * sin;                                                                        \
        dy = dx * sin + dy * cos;                                                                           \
        dx = tempx;                                                                                         \
                                                                                                            \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                              \
                                                                                                            \
        SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);\
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                                \
    }                                                                                                       \
                                                                                                            \
    dx = cosf(DegToRad(endAngle));                                                                          \
    dy = sinf(DegToRad(endAngle));                                                                          \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                                  \
                                                                                                            \
    SetUnTextureVertex((renderer)->blit_buffer,(center).x + radius * dx,(center).y + radius * dy,color);    \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                                    \
}while(0)

#define _DrawPixel(renderer,point,color)                                                        \
do{                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_POINTS);                                                  \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                     \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(point).x,(point).y,color);                     \
}while(0)

#define _DrawLine(renderer,point1,point2,color)                                                 \
do{                                                                                            \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                   \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(point1).x,(point1).y,color);                   \
    SetUnTextureVertex((renderer)->blit_buffer,(point2).x,(point2).y,color);                   \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                     \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                     \
}while(0)

#define _DrawLines(renderer,points,points_count,color)                                          \
do{                                                                                            \
    if(!points || points_count < 2) return;                                                    \
                                                                                               \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                   \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
    uint32_t current_index = start_index;                                                      \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(points)[0].x,(points)[0].y,color);             \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                     \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(points)[1].x,(points)[1].y,color);             \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                       \
                                                                                               \
    for(uint32_t i = 2; i<points_count; ++i){                                                  \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                 \
                                                                                               \
        SetUnTextureVertex((renderer)->blit_buffer,(points)[i].x,(points)[i].y,color);         \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                   \
    }                                                                                          \
}while(0)

#define _DrawPolygon(renderer,vertices,vertices_count,color)                                    \
do{                                                                                            \
    if(!vertices || vertices_count < 3) return;                                                \
                                                                                               \
    WG_PrepareToRenderShape(renderer,GL_LINES);                                                   \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
    uint32_t current_index = start_index;                                                      \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertices)[0].x,(vertices)[0].y,color);         \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                     \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertices)[1].x,(vertices)[1].y,color);         \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                       \
                                                                                               \
    for(uint32_t i=2; i<vertices_count; ++i){                                                  \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                 \
                                                                                               \
        SetUnTextureVertex((renderer)->blit_buffer,(vertices)[i].x,(vertices)[i].y,color);     \
        GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                   \
    }                                                                                          \
                                                                                               \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                       \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                         \
}while(0)

#define _DrawFilledPolygon(renderer,vertices,vertices_count,color)                              \
do{                                                                                            \
    if(!vertices || vertices_count < 3) return;                                                \
                                                                                               \
    WG_PrepareToRenderShape(renderer,GL_TRIANGLES);                                               \
                                                                                               \
    uint32_t start_index = ContentSize((renderer)->blit_buffer,float) / UnTextureAttributeSize;\
    uint32_t current_index = start_index;                                                      \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertices)[0].x,(vertices)[0].y,color);         \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                     \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertices)[1].x,(vertices)[1].y,color);         \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);                     \
                                                                                               \
    SetUnTextureVertex((renderer)->blit_buffer,(vertices)[2].x,(vertices)[2].y,color);         \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);                       \
                                                                                               \
    if(vertices_count > 3){                                                                    \
                                                                                               \
        for(uint32_t i=3; i<vertices_count; ++i){                                              \
                                                                                               \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,start_index);                 \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index++);             \
                                                                                               \
            SetUnTextureVertex((renderer)->blit_buffer,(vertices)[i].x,(vertices)[i].y,color); \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,current_index);               \
        }                                                                                      \
    }                                                                                          \
}while(0)

#define _DrawText(renderer,atlas,text,pos,color)                                                \
do{                                                                                             \
    WG_PrepareToRenderTexture(renderer);                                                        \
                                                                                                \
    WG_BindTexture(renderer,(atlas)->texture);                                                  \
                                                                                                \
    CharData char_data;                                                                         \
    float x1,y1,x2,y2;                                                                          \
    int index;                                                                                  \
    uint32_t start_index;                                                                       \
                                                                                                \
    while(*text){                                                                               \
        index = *text - (atlas)->first_char;                                                    \
        if(index < 0 || index >= (atlas)->num_chars) index = 0;                                 \
        char_data = (atlas)->char_data[index];                                                  \
                                                                                                \
        if((char_data).isValid){                                                                \
            x1 = (pos).x + (char_data).xoffset;                                                 \
            y1 = (pos).y + (char_data).yoffset;                                                 \
            x2 = x1 + (char_data).w;                                                            \
            y2 = y1 + (char_data).h;                                                            \
                                                                                                \
            start_index = ContentSize((renderer)->blit_buffer,float) / TextureAttributeSize;    \
                                                                                                \
            SetTextureVertex((renderer)->blit_buffer,x1,y1,(char_data).s0,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y1,(char_data).s1,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y2,(char_data).s1,(char_data).t1,color);\
            SetTextureVertex((renderer)->blit_buffer,x1,y2,(char_data).s0,(char_data).t1,color);\
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);              \
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
        }                                                                                       \
        (pos).x += (char_data).xadvance;                                                        \
        text++;                                                                                 \
    }                                                                                           \
}while(0)

#define _DrawNText(renderer,atlas,text,n,pos,color)                                             \
do{                                                                                             \
    WG_PrepareToRenderTexture(renderer);                                                        \
                                                                                                \
    WG_BindTexture(renderer,(atlas)->texture);                                                  \
                                                                                                \
    CharData char_data;                                                                         \
    float x1,y1,x2,y2;                                                                          \
    int index;                                                                                  \
    uint32_t start_index;                                                                       \
                                                                                                \
    for(uint32_t i=0; i<n && text[i]; ++i){                                                     \
        index = text[i] - (atlas)->first_char;                                                  \
        if(index < 0 || index >= (atlas)->num_chars) index = 0;                                 \
        char_data = (atlas)->char_data[index];                                                  \
                                                                                                \
        if((char_data).isValid){                                                                \
            x1 = (pos).x + (char_data).xoffset;                                                 \
            y1 = (pos).y + (char_data).yoffset;                                                 \
            x2 = x1 + (char_data).w;                                                            \
            y2 = y1 + (char_data).h;                                                            \
                                                                                                \
            start_index = ContentSize((renderer)->blit_buffer,float) / TextureAttributeSize;    \
                                                                                                \
            SetTextureVertex((renderer)->blit_buffer,x1,y1,(char_data).s0,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y1,(char_data).s1,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y2,(char_data).s1,(char_data).t1,color);\
            SetTextureVertex((renderer)->blit_buffer,x1,y2,(char_data).s0,(char_data).t1,color);\
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);              \
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
        }                                                                                       \
        (pos).x += (char_data).xadvance;                                                        \
    }                                                                                           \
}while(0)

#define _DrawTextByGapBuffer(renderer,atlas,gap_buffer,pos,color)                               \
do{                                                                                             \
    WG_PrepareToRenderTexture(renderer);                                                        \
                                                                                                \
    WG_BindTexture(renderer,(atlas)->texture);                                                  \
                                                                                                \
    CharData char_data;                                                                         \
    float x1,y1,x2,y2;                                                                          \
    int index;                                                                                  \
    uint32_t start_index;                                                                       \
    uint16_t *ptr = (gap_buffer)->buffer;                                                       \
                                                                                                \
    while(ptr < (uint16_t*)(gap_buffer)->buffer_end){                                           \
                                                                                                \
        if(ptr == (uint16_t*)(gap_buffer)->gap_start){                                          \
            ptr = (uint16_t*)(gap_buffer)->gap_end;                                             \
            if(ptr >= (uint16_t*)(gap_buffer)->buffer_end) break;                               \
        }                                                                                       \
                                                                                                \
        index = *ptr - (atlas)->first_char;                                                     \
        if(index < 0 || index >= (atlas)->num_chars) index = 0;                                 \
        char_data = (atlas)->char_data[index];                                                  \
                                                                                                \
        if((char_data).isValid){                                                                \
            x1 = (pos).x + (char_data).xoffset;                                                 \
            y1 = (pos).y + (char_data).yoffset;                                                 \
            x2 = x1 + (char_data).w;                                                            \
            y2 = y1 + (char_data).h;                                                            \
                                                                                                \
            start_index = ContentSize((renderer)->blit_buffer,float) / TextureAttributeSize;    \
                                                                                                \
            SetTextureVertex((renderer)->blit_buffer,x1,y1,(char_data).s0,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y1,(char_data).s1,(char_data).t0,color);\
            SetTextureVertex((renderer)->blit_buffer,x2,y2,(char_data).s1,(char_data).t1,color);\
            SetTextureVertex((renderer)->blit_buffer,x1,y2,(char_data).s0,(char_data).t1,color);\
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);              \
                                                                                                \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);              \
            GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);              \
        }                                                                                       \
        (pos).x += (char_data).xadvance;                                                        \
        ptr++;                                                                                  \
    }                                                                                           \
}while(0)

#define _Blit(renderer,texture,src_rect,dst_rect)                                                            \
do{                                                                                                         \
    WG_PrepareToRenderTexture(renderer);                                                                       \
                                                                                                            \
    WG_BindTexture(renderer,texture);                                                                          \
                                                                                                            \
    float tx1,ty1,tx2,ty2;                                                                                  \
    float vx1,vy1,vx2,vy2;                                                                                  \
    int start_index = ContentSize((renderer)->blit_buffer,float) / TextureAttributeSize;                    \
                                                                                                            \
    if(dst_rect){                                                                                           \
        vx1 = (dst_rect)->x;                                                                                \
        vy1 = (dst_rect)->y;                                                                                \
        vx2 = (dst_rect)->x + (dst_rect)->w;                                                                \
        vy2 = (dst_rect)->y + (dst_rect)->h;                                                                \
    }                                                                                                       \
    else{                                                                                                   \
        vx1 = 0.0f;                                                                                         \
        vy1 = 0.0f;                                                                                         \
        vx2 = (renderer)->current_view->viewport.w;                                                         \
        vy2 = (renderer)->current_view->viewport.h;                                                         \
    }                                                                                                       \
                                                                                                            \
    if(src_rect){                                                                                           \
        tx1 = (src_rect)->x / (float)(texture)->width;                                                      \
        ty1 = (src_rect)->y / (float)(texture)->height;                                                     \
        tx2 = ((src_rect)->x + (src_rect)->w) / (float)(texture)->width;                                    \
        ty2 = ((src_rect)->x + (src_rect)->h) / (float)(texture)->height;                                   \
    }                                                                                                       \
    else{                                                                                                   \
        tx1 = 0.0f;                                                                                         \
        ty1 = 0.0f;                                                                                         \
        tx2 = 1.0f;                                                                                         \
        ty2 = 1.0f;                                                                                         \
    }                                                                                                       \
                                                                                                            \
    SetTextureVertex((renderer)->blit_buffer,vx1,vy1,tx1,ty2,(texture)->color);                             \
    SetTextureVertex((renderer)->blit_buffer,vx2,vy1,tx2,ty2,(texture)->color);                             \
    SetTextureVertex((renderer)->blit_buffer,vx2,vy2,tx2,ty1,(texture)->color);                             \
    SetTextureVertex((renderer)->blit_buffer,vx1,vy2,tx1,ty1,(texture)->color);                             \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);                                  \
                                                                                                            \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);                                  \
}while(0)

#define _BlitEx(renderer,texture,src_rect,dst_rect,angle,center,flags)                   \
do{                                                                                     \
    WG_PrepareToRenderTexture(renderer);                                                   \
                                                                                        \
    WG_BindTexture(renderer,texture);                                                      \
                                                                                        \
    float tx1,ty1,tx2,ty2,tx3,ty3,tx4,ty4;                                              \
    float vx1,vy1,vx2,vy2,vx3,vy3,vx4,vy4;                                              \
    float cx,cy;                                                                        \
                                                                                        \
    int start_index = ContentSize((renderer)->blit_buffer,float) / TextureAttributeSize;\
                                                                                        \
    if(dst_rect){                                                                       \
        vx1 = (dst_rect)->x;                                                            \
        vy1 = (dst_rect)->y;                                                            \
        vx3 = (dst_rect)->x + (dst_rect)->w;                                            \
        vy3 = (dst_rect)->y + (dst_rect)->h;                                            \
                                                                                        \
        cx = (dst_rect)->x + (dst_rect)->w * 0.5f;                                      \
        cy = (dst_rect)->y + (dst_rect)->h * 0.5f;                                      \
    }                                                                                   \
    else{                                                                               \
        vx1 = 0.0f;                                                                     \
        vy1 = 0.0f;                                                                     \
        vx3 = (renderer)->current_view->viewport.w;                                     \
        vy3 = (renderer)->current_view->viewport.h;                                     \
                                                                                        \
        cx = vx3 * 0.5f;                                                                \
        cy = vy3 * 0.5f;                                                                \
    }                                                                                   \
                                                                                        \
    vx2 = vx3;                                                                          \
    vy2 = vy1;                                                                          \
    vx4 = vx1;                                                                          \
    vy4 = vy3;                                                                          \
                                                                                        \
    if(angle != 0.0f){                                                                  \
                                                                                        \
        float cos_a = cosf(DegToRad(angle));                                            \
        float sin_a = sinf(DegToRad(angle));                                            \
        float tempx;                                                                    \
                                                                                        \
        if(center != NULL){cx = (center)->x; cy = (center)->y;}                         \
                                                                                        \
        tempx = vx1;                                                                    \
        vx1 = roundf(((vx1 - cx) * cos_a - (vy1 - cy) * sin_a) + cx);                   \
        vy1 = roundf(((tempx - cx) * sin_a + (vy1 - cy) * cos_a) + cy);                 \
                                                                                        \
        tempx = vx2;                                                              \
        vx2 = roundf(((vx2 - cx) * cos_a - (vy2 - cy) * sin_a) + cx);             \
        vy2 = roundf(((tempx - cx) * sin_a + (vy2 - cy) * cos_a) + cy);           \
                                                                                  \
        tempx = vx3;                                                              \
        vx3 = roundf(((vx3 - cx) * cos_a - (vy3 - cy) * sin_a) + cx);             \
        vy3 = roundf(((tempx - cx) * sin_a + (vy3 - cy) * cos_a) + cy);           \
                                                                                  \
        tempx = vx4;                                                              \
        vx4 = roundf(((vx4 - cx) * cos_a - (vy4 - cy) * sin_a) + cx);             \
        vy4 = roundf(((tempx - cx) * sin_a + (vy4 - cy) * cos_a) + cy);           \
    }                                                                             \
                                                                                  \
    if(src_rect){                                                                 \
                                                                                  \
        tx1 = (src_rect)->x / (float)(texture)->width;                            \
        ty1 = (src_rect)->y / (float)(texture)->height;                           \
                                                                                  \
        tx2 = ((src_rect)->x + (src_rect)->w) / (float)(texture)->width;          \
        ty2 = (src_rect)->y / (float)(texture)->height;                           \
                                                                                  \
        tx3 = ((src_rect)->x + (src_rect)->w) / (float)(texture)->width;          \
        ty3 = ((src_rect)->x + (src_rect)->h) / (float)(texture)->height;         \
                                                                                  \
        tx4 = (src_rect)->x / (float)(texture)->width;                            \
        ty4 = ((src_rect)->x + (src_rect)->h) / (float)(texture)->height;         \
    }                                                                             \
    else{                                                                         \
        tx1 = 0.0f;                                                               \
        ty1 = 0.0f;                                                               \
                                                                                  \
        tx2 = 1.0f;                                                               \
        ty2 = 0.0f;                                                               \
                                                                                  \
        tx3 = 1.0f;                                                               \
        ty3 = 1.0f;                                                               \
                                                                                  \
        tx4 = 0.0f;                                                               \
        ty4 = 1.0f;                                                               \
    }                                                                             \
                                                                                  \
    if(flags != FLIP_NONE){                                                       \
        float tempx,tempy;                                                        \
        if(flags & FLIP_HORIZONTAL){                                              \
            tempx = tx1;                                                          \
            tx1 = tx2;                                                            \
            tx2 = tempx;                                                          \
                                                                                  \
            tempx = tx4;                                                          \
            tx4 = tx3;                                                            \
            tx3 = tempx;                                                          \
        }                                                                         \
        if(flags & FLIP_VERTICAL){                                                \
            tempy = ty1;                                                          \
            ty1 = ty4;                                                            \
            ty4 = tempy;                                                          \
                                                                                  \
            tempy = ty2;                                                          \
            ty2 = ty3;                                                            \
            ty3 = tempy;                                                          \
        }                                                                         \
    }                                                                             \
                                                                                  \
    SetTextureVertex((renderer)->blit_buffer,vx1,vy1,tx4,ty4,(texture)->color);   \
    SetTextureVertex((renderer)->blit_buffer,vx2,vy2,tx3,ty3,(texture)->color);   \
    SetTextureVertex((renderer)->blit_buffer,vx3,vy3,tx2,ty2,(texture)->color);   \
    SetTextureVertex((renderer)->blit_buffer,vx4,vy4,tx1,ty1,(texture)->color);   \
                                                                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);        \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);        \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,3 + start_index);        \
                                                                                  \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,0 + start_index);        \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,1 + start_index);        \
    GapBufferInsertValue((renderer)->blit_index,uint32_t,2 + start_index);        \
}while(0)

#endif