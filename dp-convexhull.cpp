template<typename PayloadType>
struct DpIntCht {
    struct ChtLine {
        Line line;
        Point startPoint;
        PayloadType data;
    };
    using Lines = vector<ChtLine>;
    //using LineIt = typename list<ChtLine>::iterator;

    DpIntCht()
        : linesByX(createByXComparator())
        , linesByAngle(createByAngleComparator())
    {
        //constexpr int64_t maxCoord = 1'000'000;
        constexpr double maxCoord = 1e20;
        const Point a(-maxCoord, -maxCoord);
        const Point b(-maxCoord + 1, maxCoord - 1);
        const Point c(maxCoord, maxCoord);
        addFakeLine(a, b, a * Ratio(maxCoord/*, 1*/)); // todo fix: a - (b - a) * 100500
        addFakeLine(b, c, b);
    }

    void addLine(int64_t x0, int64_t y0, int64_t x1, int64_t y1, PayloadType data) {
        lines.push_back(ChtLine{ Line(Point(x0, y0), Point(x1, y1)), Point(0, 0), data });
        Line& line = lines.back().line;
        //auto lineIt = lines.end();
        //--lineIt;
        const int lineId = static_cast<int>(lines.size()) - 1;

        auto ait = linesByAngle.upper_bound(lineId);
        --ait;

        // check for line with similar angle
        if (line.crossWith(lines[*ait].line).first == false) {
            const Point dir = line.to - line.from;
            const Ratio offkf = (lines[*ait].line.from.x - line.from.x) / dir.x;
            const Ratio lineY = (line.from + dir * offkf).y;
            if (lines[*ait].line.from.y <= lineY)
                return;
            auto nit = ait;
            --nit;
            removeLine(*ait);
            ait = nit;
        }

        // both-direction checks: find intersection point and exclude lines
        while (true) {
            auto intersectionResult = line.crossWith(lines[*ait].line);
            assert(intersectionResult.first == true);
            if (intersectionResult.second.x <= lines[*ait].startPoint.x) {
                auto nit = ait;
                --nit;
                removeLine(*ait);
                ait = nit;
            }
            else {
                lines.back().startPoint = intersectionResult.second;
                break;
            }
        }

        ++ait;
        assert(ait != linesByAngle.end());

        while (true) {
            auto nit = ait;
            ++nit;

            auto intersectionResult = line.crossWith(lines[*ait].line);
            assert(intersectionResult.first == true);
            lines[*ait].startPoint = intersectionResult.second;

            if (nit == linesByAngle.end())
                break;

            if (lines[*nit].startPoint.x <= intersectionResult.second.x) {
                removeLine(*ait);
                ait = nit;
            }
            else {
                break;
            }
        }

        linesByAngle.insert(lineId);
        linesByX.insert(lineId);
    }

    PayloadType& findLineByX(int64_t x) {
        const Point fakePoint(0, 0);
        const Line fakeLine(fakePoint, fakePoint);
        const ChtLine line{ fakeLine, Point(x, 0), {} };
        lines.push_back(line);
        auto it = linesByX.upper_bound(static_cast<int>(lines.size()) - 1);
        --it;
        const int resultId = *it;
        lines.pop_back();
        return lines[resultId].data;
    }

    void print() const {
        for (const int id : linesByAngle) {
            const ChtLine& line = lines[id];
            E(line.startPoint.x);
            E(line.line);
            Eo(".");
        }
    }

private:
    void addFakeLine(const Point& from, const Point& to, const Point& startPoint) {
        ChtLine fake{ Line(from, to), startPoint, {} };
        lines.push_back(fake);
        //LineIt pline = lines.end();
        //--pline;
        const int lineId = static_cast<int>(lines.size()) - 1;
        linesByX.insert(lineId);
        linesByAngle.insert(lineId);
    }

    void removeLine(int it) {
        auto xit = linesByX.find(it);
        linesByX.erase(xit);
        auto ait = linesByAngle.find(it);
        linesByAngle.erase(ait);
        //lines.erase(it);
    }

    struct CompareByX {
        CompareByX(vector<ChtLine>& l) : lines(l) {}

        bool operator()(int left, int right) const {
            return lines[left].startPoint.x < lines[right].startPoint.x;
        }

        vector<ChtLine>& lines;
    };
    CompareByX createByXComparator() {
        return CompareByX(lines);
    }

    struct CompareByAngle {
        CompareByAngle(vector<ChtLine>& l) : lines(l) {}

        bool operator()(int left, int right) const {
            const Point ldir = lines[left].line.to - lines[left].line.from;
            const Point rdir = lines[right].line.to - lines[right].line.from;
            const Ratio dotprod = ldir.x * rdir.y - ldir.y * rdir.x;
#if 0
            return dotprod.up < 0;
#else
            return dotprod < 0;
#endif
        }
      
        vector<ChtLine>& lines;
    };
    CompareByAngle createByAngleComparator() {
        return CompareByAngle(lines);
    }

    vector<ChtLine> lines;
    set<int, CompareByX> linesByX;
    set<int, CompareByAngle> linesByAngle;
};
