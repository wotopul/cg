#pragma once

#include <QtOpenGL>

namespace visualization
{
    struct client_data_accumulator_t
    {
        virtual void accumulate() = 0;
        virtual ~client_data_accumulator_t() {}

        struct point_buffer_t
        {
            std::vector<GLfloat>    points;
            QColor                  color;
            float radius;
        };

        struct segment_buffer_t
        {
            std::vector<GLfloat>    segments;
            QColor                  color;
            float width;
        };

        struct string_to_print
        {
            dpoint      pos;
            std::string text;
        };

        std::vector<point_buffer_t>     point_buffers;
        std::vector<segment_buffer_t>   segment_buffers;
        std::vector<string_to_print>    strings;
    };
}

