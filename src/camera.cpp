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

    m_cameraCenter = m_cameraProps.lookFrom;

    const auto theta = degreesToRadians(m_cameraProps.vFov);
    const auto height = std::tan(theta / 2);
    const auto viewportHeight = 2.0 * height * m_cameraProps.focusDist;
    const auto viewportWidth =
        viewportHeight * (static_cast<double>(m_cameraProps.imageWidth) /
                          static_cast<double>(m_imageHeight));

    m_cameraW = unitVector(m_cameraProps.lookFrom - m_cameraProps.lookAt);
    m_cameraU = unitVector(cross(m_cameraProps.vUp, m_cameraW));
    m_cameraV = cross(m_cameraW, m_cameraU);

    const auto viewportU = viewportWidth * m_cameraU;
    const auto viewportV = viewportHeight * -m_cameraV;

    m_pixelDeltaU = viewportU / static_cast<double>(m_cameraProps.imageWidth);
    m_pixelDeltaV = viewportV / static_cast<double>(m_imageHeight);

    const auto viewportUpperLeft = m_cameraCenter -
                                   m_cameraProps.focusDist * m_cameraW -
                                   viewportU / 2 - viewportV / 2;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    const auto defocusRadius =
        m_cameraProps.focusDist *
        std::tan(degreesToRadians(m_cameraProps.defocusAngle / 2));

    m_defocusDiskU = m_cameraU * defocusRadius;
    m_defocusDiskV = m_cameraV * defocusRadius;

    // Renderer.
    m_image = cimg_library::CImg<uint8_t>(
        static_cast<unsigned int>(m_cameraProps.imageWidth),
        static_cast<unsigned int>(m_imageHeight), 1u, 3u, 0u);
}
