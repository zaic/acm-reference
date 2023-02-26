struct Point {
    Ratio x;
    Ratio y;

#if 0
    Point(int64_t xx, int64_t yy) : x(Ratio(xx, 1)), y(Ratio(yy, 1)) {}
#endif
    Point(const Ratio& xx, const Ratio& yy) : x(xx), y(yy) {}

    Point operator*(const Ratio& value) const {
        return Point(x * value, y * value);
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
};

ostream& operator<<(ostream& os, const Point& point) {
    return os << "{x=" << point.x << ", y=" << point.y << "}";
}



struct Line {
    Point from;
    Point to;

    Line(const Point& f, const Point& t) : from(f), to(t) {}

    pair<bool, Point> crossWith(const Line& other) const {
        const Point& a1 = from;
        const Point& b1 = to;
        const Point& a2 = other.from;
        const Point& b2 = other.to;
        // up=c, down=1
        Ratio a11 = b1.x - a1.x;
        Ratio a12 = a2.x - b2.x;
        Ratio a21 = b1.y - a1.y;
        Ratio a22 = a2.y - b2.y;
        Ratio xb1 = a2.x - a1.x;
        Ratio xb2 = a2.y - a1.y;
        // up=c^2, down=1
        Ratio det = a11 * a22 - a12 * a21;
        Ratio detu = xb1 * a22 - a12 * xb2;
        Ratio detv = a11 * xb2 - xb1 * a21;
#if 0
        if (det.up == 0)
#else
        if (abs(det) < 1e-6)
#endif
            return make_pair(false, Point(0, 0));
        // up=c^2, down=c^2
        detu = detu / det;
        detv = detv / det;
        // up=c, down=1
        const Point dir = b1 - a1;
        // up=c^3, down=c^2
        const Point dirk = dir * detu;
        // up=c^3, down=c^2
        const Point c = a1 + dirk;

        return make_pair(true, c);
    }
};

ostream& operator<<(ostream& os, const Line& line) {
    return os << "{from=" << line.from << ", to=" << line.to << "}";
}
