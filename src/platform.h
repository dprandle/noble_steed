#pragma once

#include "nsmemory.h"
#include "build_and_battle.h"
#include "nsstring.h"
#include "math/vector2.h"

struct GLFWwindow;
namespace noble_steed
{

struct bb_state;
struct window;

enum struct platform_error_t
{
    NONE,
    FILE_SIZE,
    FILE_READ,
    FILE_WRITE
};

enum struct window_type
{
    GLFW
};

struct window_glfw
{
    GLFWwindow *hndl;
};

struct window
{
    window_type type;
    ivec2 current_size;
    vec2 norm_mpos;
    union
    {
        window_glfw wglfw;
    };
};

struct window_flags
{
    enum
    {
        VISIBLE = 1,          // Ignored for full screen windows
        INTIALLY_FOCUSED = 2, // Ignored for full screen and initially hidden windows
        DECORATED = 4,        // Ignored for full screen windows
        MAXIMIZE = 8,         // Ignored for full screen
        ALWAYS_ON_TOP = 16,   // Ignored for full screen
        FULLSCREEN = 32,
        FULLSCREEN_AUTO_ICONIFTY = 64,  // Ignored for non full screen windows
        FULLSCREEN_CENTER_CURSOR = 128, // Ignored for non full screen windows
        SCALE_TO_MONITOR = 256
    };
};

struct window_create_params
{
    i16 win_flags;
    ivec2 resolution;
    small_str title;
};

struct platform_ctxt
{
    window win;
    bb_state bstate;
    mem_store mem;
    mem_store frame_mem;
};

namespace err_code
{
constexpr inline const int FOPEN_FAIL = 1;
constexpr inline const int FTELL_FAIL = 2;
constexpr inline const int FSEEK_FAIL = 3;
constexpr inline const int FREAD_DIFF_SIZE = 4;
constexpr inline const int FWRITE_DIFF_SIZE = 5;
constexpr inline const int GET_CWD_FAIL = 6;
}

err_desc platform_cwd(path_str * pstr);

result_err_desc<sizet> platform_file_size(FILE *f);

template<class T> 
result_err_desc<sizet> platform_read_file(FILE *f, T *data, sizet nelements, sizet byte_offset=0)
{
    result_err_desc<sizet> ret {};
    if (byte_offset != 0 && fseek(f, byte_offset, SEEK_SET) != 0)
    {
        ret.err.type = err_code::FSEEK_FAIL;
        ret.err.code = errno;
        return ret;
    }
    
    ret.val = fread(data, sizeof(T), nelements, f);
    if (ret.val != nelements)
    {
        ret.err.type = err_code::FREAD_DIFF_SIZE;
        ret.err.code = errno;
    }
    return ret;
}

template<class T> 
result_err_desc<sizet> platform_read_file(const char *fname, T *data, sizet nelements, sizet byte_offset=0)
{
    result_err_desc<sizet> ret {};
    FILE *f = fopen(fname, "r");
    if (!f)
    {
        ret.err.type = err_code::FOPEN_FAIL;
        ret.err.code = errno;
        return ret;
    }
    ret = platform_read_file(f, data, nelements, byte_offset);
    fclose(f);
    return ret;
}

// If 0, vec will be resized to entire file size
template<class T> 
result_err_desc<sizet> platform_read_file(const char *fname, vector<T> *vec, sizet byte_offset=0)
{
    result_err_desc<sizet> ret {};
    FILE *f = fopen(fname, "r");
    if (!f)
    {
        ret.err.type = err_code::FOPEN_FAIL;
        ret.err.code = errno;
        return ret;
    }

    ret = platform_file_size(f);
    if (error_occured(ret.err))
    {
        fclose(f);
        return ret;
    }

    // max elements - if size is 0 or above the max, resize the vector to the max
    // otherwise use the vector count as element count
    sizet nelements = (ret.val - byte_offset) / sizeof(T);
    if (vec->size() == 0 || vec->size() > nelements)
        resize(vec, nelements);
    else
        nelements = vec->size();
    
    ret = platform_read_file(f, vec->buf.data, byte_offset);
    fclose(f);
    return ret;
}

template<class T>
result_err_desc<sizet> platform_write_file(FILE * f, const T *data, sizet nelements, sizet noffset=0)
{
    result_err_desc<sizet> ret {};
    int seek = SEEK_SET;
    if (noffset == -1)
    {
        noffset = 0;
        seek = SEEK_END;
    }

    if ((noffset != 0 || seek != SEEK_SET) && fseek(f, noffset*sizeof(T), seek) != 0)
    {
        ret.err.type = err_code::FSEEK_FAIL;
        ret.err.code = errno;
        return ret;
    }

    ret.val = fwrite(data, sizeof(T), nelements, f);
    if (ret.val != nelements)
    {
        ret.err.type = err_code::FWRITE_DIFF_SIZE;
        ret.err.code = errno;
    }
    return ret;
}

template<class T>
result_err_desc<sizet> platform_write_file(const char *fname, bool create_if_doesnt_exist, const T *data, sizet nelements, sizet noffset=0)
{
    result_err_desc<sizet> ret {};

    char fm[] = "r+";
    if (create_if_doesnt_exist)
    {
        fm[1] = 0;
        if (noffset != -1)
            fm[0] = 'w';
        else
        {
            noffset = 0;
            fm[0] = 'a';
        }
    }

    FILE *f = fopen(fname, fm);
    if (!f)
    {
        ret.err.type = err_code::FOPEN_FAIL;
        ret.err.code = errno;
        return ret;
    }
    ret = platform_write_file(f, data, nelements, noffset);
    fclose(f);
    return ret;
}

template<class T> 
result_err_desc<sizet> platform_write_file(const char *fname, bool create_if_doesnt_exist, const vector<T> &data, sizet noffset=0)
{
    return platform_write_file(fname, create_if_doesnt_exist, data.buf.data, data.size(), noffset);
}

void platform_init(platform_ctxt *pform, window_create_params cp);
void platform_exec(platform_ctxt *pform);
void platform_term(platform_ctxt *pform);

} // namespace noble_steed