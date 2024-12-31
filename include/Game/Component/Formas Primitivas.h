#include <GL/glew.h>
#include <cmath>  

static void draw_cube() {
    glBegin(GL_TRIANGLES);

    // Cara frontal
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, 1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, -1.f, 1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, 1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.f, 1.f, 1.f);

    // Cara trasera
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, -1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, -1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.f, 1.f, -1.f);

    // Cara izquierda
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.f, -1.f, 1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.f, 1.f, 1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.f, 1.f, 1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.f, 1.f, -1.f);

    // Cara derecha
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, -1.f, 1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.f, 1.f, -1.f);

    // Cara superior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, 1.f, -1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, 1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, 1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.f, 1.f, 1.f);

    // Cara inferior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, -1.f, 1.f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.f, -1.f, -1.f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, -1.f, 1.f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.f, -1.f, 1.f);

    glEnd();
}
static void draw_triangle() {
    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f); // Vértice superior
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // Vértice inferior izquierdo
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);  // Vértice inferior derecho

    glEnd();
}
static void draw_circle(float radius = 1.0f, int segments = 36) {
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        glTexCoord2f(0.5f, 0.5f); // Centro
        glVertex3f(0.0f, 0.0f, 0.0f);

        glTexCoord2f(0.5f + 0.5f * cos(angle1), 0.5f + 0.5f * sin(angle1));
        glVertex3f(radius * cos(angle1), radius * sin(angle1), 0.0f);

        glTexCoord2f(0.5f + 0.5f * cos(angle2), 0.5f + 0.5f * sin(angle2));
        glVertex3f(radius * cos(angle2), radius * sin(angle2), 0.0f);
    }

    glEnd();
}
static void draw_cylinder(float radius = 1.0f, float height = 2.0f, int segments = 36) {
    float halfHeight = height / 2.0f;

    // Tapa superior
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, halfHeight, 0.0f);

        glTexCoord2f(0.5f + 0.5f * cos(angle1), 0.5f + 0.5f * sin(angle1));
        glVertex3f(radius * cos(angle1), halfHeight, radius * sin(angle1));

        glTexCoord2f(0.5f + 0.5f * cos(angle2), 0.5f + 0.5f * sin(angle2));
        glVertex3f(radius * cos(angle2), halfHeight, radius * sin(angle2));
    }
    glEnd();

    // Tapa inferior
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -halfHeight, 0.0f);

        glTexCoord2f(0.5f + 0.5f * cos(angle1), 0.5f + 0.5f * sin(angle1));
        glVertex3f(radius * cos(angle1), -halfHeight, radius * sin(angle1));

        glTexCoord2f(0.5f + 0.5f * cos(angle2), 0.5f + 0.5f * sin(angle2));
        glVertex3f(radius * cos(angle2), -halfHeight, radius * sin(angle2));
    }
    glEnd();

    // Superficie lateral
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        glTexCoord2f((float)i / segments, 1.0f);
        glVertex3f(radius * cos(angle1), halfHeight, radius * sin(angle1));

        glTexCoord2f((float)i / segments, 0.0f);
        glVertex3f(radius * cos(angle1), -halfHeight, radius * sin(angle1));

        glTexCoord2f((float)(i + 1) / segments, 1.0f);
        glVertex3f(radius * cos(angle2), halfHeight, radius * sin(angle2));

        glTexCoord2f((float)i / segments, 0.0f);
        glVertex3f(radius * cos(angle1), -halfHeight, radius * sin(angle1));

        glTexCoord2f((float)(i + 1) / segments, 0.0f);
        glVertex3f(radius * cos(angle2), -halfHeight, radius * sin(angle2));

        glTexCoord2f((float)(i + 1) / segments, 1.0f);
        glVertex3f(radius * cos(angle2), halfHeight, radius * sin(angle2));
    }
    glEnd();
}