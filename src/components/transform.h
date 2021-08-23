#pragma once

#include <glm/glm.hpp>

namespace components {

    // when working with the transform component, it should be noted that affine transforms may not
    // preserve orthogonality, while rotations and translations are always orthogonal, non-uniform
    // scaling and shearing are not. In particular, if the transform matrix is not orthogonal, the
    // transformed normal vectors cannot be computed by simply multiplying with the matrix itself,
    // but instead needs the transpose of the inverse of it, otherwise tangents may break as well.
    // if you need complex transformations, it's probably better to write a geometry shader.

    class Transform {
      private:
        void RecalculateBasis(void);

      public:
        Transform();
        ~Transform() {}

        Transform(const Transform&) = delete;
        Transform& operator=(const Transform&) = delete;

        Transform(Transform&& other) = default;
        Transform& operator=(Transform&& other) = default;

      public:
        glm::vec3 position;
        glm::vec3 rotation;   // euler angles in degrees (pitch, yawn, roll)
        glm::vec3 scale;

        glm::mat4 transform;  // 4x4 homogeneous matrix stored in column-major order

        glm::vec3 up;         // local up direction
        glm::vec3 forward;    // local forward direction
        glm::vec3 right;      // local right direction

        // transformation functions
        void Translate(const glm::vec3& vec);
        void Rotate(float radians, const glm::vec3& axis);
        void Rotate(const glm::vec3& degrees);
        void Scale(float factor);
    };

}
