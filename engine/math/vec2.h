class vec2 {
public:
    float x, y;

    vec2() : x(0.0f), y(0.0f) {} 
    vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	vec2(const vec2& v) {
		this->x = v.x;
		this->y = v.y;
	}
};