/*
model_type: KANNALA_BRANDT
projection_parameters
*/

extern "C"
{
    enum KB_STATUS
    {
        KB_ER = -1, //error
        KB_OK = 0,  //projection_parameters is OK
        KB_CR = 1,  //projection_parameters out range
        KB_CN = 2,  //projection_parameters is not convergence
        KB_TH = 3,  //projection_parameters can not get theta
    };

    const char *kbGetVersion();

    /*
    radius is differece and dynamic:
        cvmodule: 350
        cv2: 350
        G2: 300
        newG2: 320
    */
    int kbSetThetaRadius(double radius);

    KB_STATUS kbTestParameters(int image_width,
                               int image_height,
                               double k2,
                               double k3,
                               double k4,
                               double k5,
                               double mu,
                               double mv,
                               double u0,
                               double v0);

    KB_STATUS kbTestFile(const char *yaml_file); //yaml_file is yaml file with stARKit format

    int kbGetThetaMap(const char* save_path);// a png image saved with radius
}