#ifndef MOTIONTRACKER_H
#define MOTIONTRACKER_H

// cv::cv2eigen 을 쓰기 위해서는 반드시 아래 순서로 include 해야 한다.
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>
#include <opencv2/core/eigen.hpp>
#include <vector>
#include "deschandler.h"
#include "pose6dofqt.h"
#include "glvertexmanager.h"


class MotionTracker
{
public:
    MotionTracker();
    void setup();
    void run();

private:
    Pose6DofQt computePoseAndPoints(cv::Mat curframe, std::vector<cv::Point3f> &recon3Dpts);
    Pose6DofQt toPoseVector(cv::Mat R, cv::Mat t);
    bool beforeSetup();
    Pose6DofQt computePoseByProjection(cv::Mat curframe,
            const std::vector<cv::Point3f>& points3D);
    void showImage(cv::Mat image, std::string wndname, int height);
    void draw3DPose(const Pose6DofQt& curPose);

    cv::VideoCapture cap;
    cv::Mat keyframe;
    cv::Mat curframe;
    DescHandler* keyfDesc;
    DescHandler* curfDesc;
    std::vector<cv::Point3f> points3D;
    cv::Mat camK;
    Pose6DofQt curPose;
};

#endif // MOTIONTRACKER_H
