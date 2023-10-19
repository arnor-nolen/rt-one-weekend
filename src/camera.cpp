#include <camera.hpp>
#include <utility>

Camera::Camera(CameraProps cameraProps)
    : m_cameraProps{std::move(cameraProps)} {
    initialize();
}

void Camera::initialize() {
    m_imageHeight =
        static_cast<size_t>(static_cast<double>(m_cameraProps.imageWidth) /
                            m_cameraProps.aspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    constexpr auto focalLength = 1.0;
    constexpr auto viewportHeight = 2.0;
    const auto viewportWidth =
        viewportHeight * (static_cast<double>(m_cameraProps.imageWidth) /
                          static_cast<double>(m_imageHeight));
    m_cameraCenter = Point3{};

    auto viewportU = Vec3{viewportWidth, 0, 0};
    auto viewportV = Vec3{0, -viewportHeight, 0};

    m_pixelDeltaU = viewportU / static_cast<double>(m_cameraProps.imageWidth);
    m_pixelDeltaV = viewportV / static_cast<double>(m_imageHeight);

    auto viewportUpperLeft = m_cameraCenter - Vec3{0, 0, focalLength} -
                             viewportU / 2 - viewportV / 2;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    // Renderer.
    m_image = cimg_library::CImg<uint8_t>(
        static_cast<unsigned int>(m_cameraProps.imageWidth),
        static_cast<unsigned int>(m_imageHeight), 1u, 3u, 0u);
}
