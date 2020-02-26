#include <cmath>
#include <string>
#include <vector>
class MiniCircl
{
    //Ref:http://www.cppblog.com/wicbnu/archive/2010/02/21/108135.html
private:
    struct pointset
    {
        double x, y;
    };
    const double precison = 1.0e-8;
    pointset maxcic;
    std::vector<pointset> point;
    double radius;
    std::vector<int> curset;
    int posset[3];
    int set_cnt, pos_cnt;

    inline double dis_2(pointset &from, pointset &to)
    {
        return ((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
    }
    int in_cic(int pt)
    {
        if (sqrt(dis_2(maxcic, point[pt])) < radius + precison)
            return 1;
        return 0;
    }
    int cal_mincic()
    {
        if (pos_cnt == 1 || pos_cnt == 0)
            return 0;
        else if (pos_cnt == 3)
        {
            double A1, B1, C1, A2, B2, C2;
            int t0 = posset[0], t1 = posset[1], t2 = posset[2];
            A1 = 2 * (point[t1].x - point[t0].x);
            B1 = 2 * (point[t1].y - point[t0].y);
            C1 = point[t1].x * point[t1].x - point[t0].x * point[t0].x +
                 point[t1].y * point[t1].y - point[t0].y * point[t0].y;
            A2 = 2 * (point[t2].x - point[t0].x);
            B2 = 2 * (point[t2].y - point[t0].y);
            C2 = point[t2].x * point[t2].x - point[t0].x * point[t0].x +
                 point[t2].y * point[t2].y - point[t0].y * point[t0].y;
            maxcic.y = (C1 * A2 - C2 * A1) / (A2 * B1 - A1 * B2);
            maxcic.x = (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
            radius = sqrt(dis_2(maxcic, point[t0]));
        }
        else if (pos_cnt == 2)
        {
            maxcic.x = (point[posset[0]].x + point[posset[1]].x) / 2;
            maxcic.y = (point[posset[0]].y + point[posset[1]].y) / 2;
            radius = sqrt(dis_2(point[posset[0]], point[posset[1]])) / 2;
        }
        return 1;
    }

    int mindisk()
    {
        if (set_cnt == 0)
        {
            return -1;
        }

        if (set_cnt == 1)
        {
            maxcic.x = point[0].x;
            maxcic.y = point[0].y;
            radius = 0;
            return 1;
        }
        if (set_cnt == 0 || pos_cnt == 3)
        {
            return cal_mincic();
        }
        int tt = curset[--set_cnt];
        int res = mindisk();
        set_cnt++;
        if (!res || !in_cic(tt))
        {
            set_cnt--;
            posset[pos_cnt++] = curset[set_cnt];
            res = mindisk();
            pos_cnt--;
            curset[set_cnt++] = curset[0];
            curset[0] = tt;
        }
        return res;
    }

public:
    MiniCircl(/* args */)
    {
        radius = -1.0;
        maxcic.x = -1;
        maxcic.y = -1;
        set_cnt = 0;
        pos_cnt = 0;
    }
    int addPoint(double x, double y)
    {
        pointset tp;
        tp.x = x;
        tp.y = y;
        point.push_back(tp);
        set_cnt = point.size();
        curset.push_back(set_cnt - 1);
    }
    int addPoint(std::vector<double> xs, std::vector<double> ys)
    {
        for (size_t i = 0; i < xs.size(); i++)
        {
            pointset tp;
            tp.x = xs[i];
            tp.y = ys[i];
            point.push_back(tp);
            curset.push_back(set_cnt + i);
        }
        set_cnt = point.size();
    }
    int getRadius(double &_radius)
    {
        int re = mindisk();
        _radius = radius;
        return re;
    }
    int getCenter(double &x, double &y)
    {
        x = maxcic.x;
        y = maxcic.y;
        return 0;
    }
};

// #define MAXN 20

// int main()
// {
//     freopen("in2.txt", "r", stdin);
//     // freopen("radius.txt", "w", stdout);
//     int n;
//     MiniCircl cc;
//     while (scanf("%d", &n) != EOF)
//     {
//         if (n == 0)
//             break;
//         int i;
//         for (i = 0; i < n; i++)
//         {
//             double x, y;
//             scanf("%lf %lf", &x, &y);
//             cc.addPoint(x, y);
//         }
//         double r;
//         cc.getRedius(r);
//         printf("%lf", r);
//         // scanf("%lf %lf", &point[i].x, &point[i].y);
//         // if (n == 1)
//         // {
//         //     maxcic.x = point[0].x;
//         //     maxcic.y = point[0].y;
//         //     radius = 0;
//         //     printf("%.2lf %.2lf %.2lf\n", maxcic.x, maxcic.y, radius);
//         //     continue;
//         // }
//         // set_cnt = n;
//         // pos_cnt = 0;
//         // for (i = 0; i < n; i++)
//         //     curset[i] = i;
//         // mindisk();
//         // printf("%.2lf %.2lf %.2lf\n", maxcic.x, maxcic.y, radius);
//     }

//     return 0;
// }