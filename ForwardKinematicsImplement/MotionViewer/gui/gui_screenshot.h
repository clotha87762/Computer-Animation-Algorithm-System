#ifndef _GUI_SCREENSHOT_H_
#define _GUI_SCREENSHOT_H_

#include "global_type.h"
#include <string>
#include <vector>
#include <memory>
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"

namespace gui {

class Screenshot final : boost::noncopyable
{

public:

    Screenshot();
    virtual ~Screenshot();
    /**
     * \brief Default zero padding digit of screenshot files
     * \return Default value of zero padding digit
     */
    static int32_t DefaultZeroPadDigit();
    /**
     * \brief Get width of screenshot
     * \return Image width
     */
    int32_t width() const;
    /**
     * \brief Get height of screenshot
     * \return Image height
     */
    int32_t height() const;
    /**
     * \brief Get zero padding digit of screenshot files
     * \return Zero padding digit
     */
    int32_t zero_pad_digit() const;
    /**
     * \brief Set the saved file name
     * The file name would be file_name0001.png, file_name0002.png......
     * if no any setting, default name is 0001.png, 0002.png..
     * \param[in] file_name
     */
    void set_file_name(const std::string &file_name);
    /**
     * \brief
     * \param[in] digit
     */
    void set_zero_pad_digit(const int32_t digit);
    /**
     * \brief Set the screenshot size
     * \param[in] width
     * \param[in] height
     */
    void SetSize(
            const int32_t width,
            const int32_t height
            );
    /**
     * \brief Save the screenshot to PNG file
     * \return Success or failure of screenshot saving
     * \retval TRUE Success to save screenshot according to the given file name
     * \retval FALSE Failure to save screenshot
     */
    bool Record();

protected:

private:

    typedef unsigned char Intensity_t;
    typedef std::vector<Intensity_t> ColorData_t;

    static int32_t PngChannelNum();
    std::string FileName();
    std::string ZeroPadNum(const int32_t num);
    void ToImageColorFormat(
            const ColorData_t &screen_color,
            ColorData_t *image_color
            );
    bool SavePNG(
            const std::string &file_name,
            const ColorData_t &image_color,
            const int32_t image_width,
            const int32_t image_height
            );

    int32_t count_;
    int32_t zero_pad_digit_;
    boost::optional<int32_t> screen_width_;
    boost::optional<int32_t> screen_height_;
    // Screen color, RGBA
    std::unique_ptr<ColorData_t> screen_color_;
    std::unique_ptr<ColorData_t> image_color_;
    std::unique_ptr<std::string> file_name_;
};

} // namespace gui {

#endif // #ifndef _GUI_SCREENSHOT_H_
