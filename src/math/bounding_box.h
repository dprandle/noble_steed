#include "../container/vector.h"
#include "nsvec3.h"
#include "nsmat4.h"

namespace noble_steed
{

template<class T>
struct bounding_box
{
    enum box_face
    {
        f_none,
        f_bottom,
        f_top,
        f_left,
        f_right,
        f_back,
        f_front
    };

    bounding_box(const vector<nsvec3<T>> &verts_ = vector<nsvec3<T>>(), const nsmat4<T> &tform_=nsmat4<T>())
    {
        calculate(verts_, tform_);
    }

    bounding_box(const nsvec3<T> &min_, const nsvec3<T> &max_): min(min_), max(max_)
    {
        update_verts();
    }

    ~bounding_box()
    {}

    void calculate(const vector<nsvec3<T>> &verts_, const nsmat4<T> &tform_ = nsmat4<T>())
    {
        clear();
        extend(verts_, tform_);
    }

    nsvec3<T> center(const box_face &face_ = f_none)
    {
        nsvec3<T> center = (max - min) * 0.5;
        switch (face_)
        {
        case (f_none):
            break;
        case (f_bottom):
            center.z = min.z;
            break;
        case (f_top):
            center.z = max.z;
            break;
        case (f_left):
            center.x = min.x;
            break;
        case (f_right):
            center.x = max.x;
            break;
        case (f_back):
            center.y = min.y;
            break;
        case (f_front):
            center.y = max.y;
            break;
        }
        return center;
    }

    void clear()
    {
        min = nsvec3<T>();
        max = nsvec3<T>();
        for (sizet i = 0; i < 8; ++i)
            verts[i] = nsvec3<T>();
    }

    void extend(const vector<nsvec3<T>> &verts_, const nsmat4<T> &tform_ = nsmat4<T>())
    {
        if (!verts_.empty())
        {
            min = verts_[0];
            max = min;
        }
        
        for (sizet i = 0; i < verts_.size(); ++i)
        {
            nsvec3<T> tvert = (tform_ * nsvec4<T>(verts_[i], 1.0f)).vec3();

            // Find maximum of each dimension
            max.maximize(tvert);

            // Find minimum of each dimension
            min.minimize(tvert);
        }
        update_verts();
    }

    T volume()
    {
        nsvec3<T> diff = (max - min).abs();
        return diff.x * diff.y * diff.z;
    }

    void update_verts()
    {
        verts[0] = min;
        verts[1] = nsvec3<T>(max.x, min.y, min.z);
        verts[2] = nsvec3<T>(min.x, max.y, min.z);
        verts[3] = nsvec3<T>(max.x, max.y, min.z);
        verts[4] = nsvec3<T>(min.x, min.y, max.z);
        verts[5] = nsvec3<T>(max.x, min.y, max.z);
        verts[6] = nsvec3<T>(min.x, max.y, max.z);
        verts[7] = max;
    }

    nsvec3<T> min;
    nsvec3<T> max;
    nsvec3<T> verts[8];
};

using cbbox = bounding_box<char>;
using c16bbox = bounding_box<char16>;
using c32bbox = bounding_box<char32>;
using cwbbox = bounding_box<wchar>;
using i8bbox = bounding_box<i8>;
using i16bbox = bounding_box<i16>;
using ibbox = bounding_box<i32>;
using i64bbox = bounding_box<i64>;
using ui8bbox = bounding_box<u8>;
using ui16bbox = bounding_box<u16>;
using uibbox = bounding_box<u32>;
using ui64bbox = bounding_box<u64>;
using bbox = bounding_box<float>;
using dbbox = bounding_box<double>;
using ldbbox = bounding_box<ldouble>;

} // namespace noble_steed