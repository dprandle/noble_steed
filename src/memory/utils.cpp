#include "utils.h"
namespace noble_steed::memory::util
{

sizet calc_padding(sizet base_addr, sizet alignment)
{
    sizet multiplier = (base_addr / alignment) + 1;
    sizet aligned_addr = multiplier * alignment;
    sizet padding = aligned_addr - base_addr;
    return padding;
}

sizet calc_padding_with_header(sizet base_addr, sizet alignment, sizet header_size)
{
    sizet padding = calc_padding(base_addr, alignment);
    sizet needed_space = header_size;

    if (padding < needed_space)
    {
        // Header does not fit - Calculate next aligned address that header fits
        needed_space -= padding;

        // How many alignments I need to fit the header
        if (needed_space % alignment > 0)
        {
            padding += alignment * (1 + (needed_space / alignment));
        }
        else
        {
            padding += alignment * (needed_space / alignment);
        }
    }

    return padding;
}

}