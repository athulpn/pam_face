#include <stdio.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "face_recog.h"

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	printf("Acct mgmt\n");
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
    int retval;

	const char* pUsername;
	retval = pam_get_user(pamh, &pUsername, "Username: ");

	printf("Welcome %s\n", pUsername);
	frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp;
    deserialize("/home/athul/.config/i3-facelock/model/shape_predictor_5_face_landmarks.dat") >> sp;
    anet_type net;
    deserialize("/home/athul/.config/i3-facelock//model/dlib_face_recognition_resnet_model_v1.dat") >> net;
	matrix<rgb_pixel> img = capture_from_cam();
    bool result = predict(get_embedding(img, net, sp, detector), 0.5);
	if (retval != PAM_SUCCESS) {
		return retval;
	}
	return result?PAM_SUCCESS:PAM_AUTH_ERR;
}