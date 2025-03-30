#include "ring.h"
#include "encoder.h"
#include "motor.h"
enum circle_type_e circle_type = CIRCLE_NONE;
// �����������ڷ�ֹһЩ�ظ������ȡ�
int16 circle_encoder;

int none_left_line = 0, none_right_line = 0;
int have_left_line = 0, have_right_line = 0;

void check_circle() {
    // ��Բ��ģʽ�£�����L�ǵ�, ���߳�ֱ��
    if (circle_type == CIRCLE_NONE && Lpt0_found && !Lpt1_found && is_straight1) {
        circle_type = CIRCLE_LEFT_BEGIN;
    }
    if (circle_type == CIRCLE_NONE && !Lpt0_found && Lpt1_found && is_straight0) {
        circle_type = CIRCLE_RIGHT_BEGIN;
    }
}
int16 current_encoder;
void run_circle() {
    current_encoder+= get_encoder();
		if(circle_type == CIRCLE_NONE)
			track_type = TRACK_MIX;
    // �󻷿�ʼ��Ѱ��ֱ������
		else if (circle_type == CIRCLE_LEFT_BEGIN) {
			track_type = TRACK_RIGHT;

        //�ȶ����ߺ�����
        if (rpts0s_num < 0.5 / sample_dist) { none_left_line++; }
        if (rpts0s_num > 1 / sample_dist && none_left_line > 2) {
            have_left_line++;
            if (have_left_line > 1) {
                circle_type = CIRCLE_LEFT_IN;
                none_left_line = 0;
                have_left_line = 0;
                circle_encoder = current_encoder;
            }
        }
    }
    //�뻷��Ѱ��Բ����
    else if (circle_type == CIRCLE_LEFT_IN) {
        track_type = TRACK_LEFT;
				int16 a=current_encoder - circle_encoder;			
        //����������1/4Բ   Ӧ����Ϊ����Ϊת���޹յ�
        if (abs(a)>23000)
				{ 
					ips200_show_uint(0,0,a,5);					
					while(circle_type!=CIRCLE_LEFT_RUNNING)
					circle_type = CIRCLE_LEFT_RUNNING;
				}
				}
    //����Ѳ�ߣ�Ѱ��Բ����
    else if (circle_type == CIRCLE_LEFT_RUNNING) {
        track_type = TRACK_RIGHT;

        if (Lpt1_found) rpts1s_num = rptsc1_num = Lpt1_rpts1s_id;
        //�⻷�յ�(��L��)
        if (Lpt1_found && Lpt1_rpts1s_id < 0.4 / sample_dist) {
            circle_type = CIRCLE_LEFT_OUT;
        }
    }
    //������Ѱ��Բ
    else if (circle_type == CIRCLE_LEFT_OUT) {
        track_type = TRACK_LEFT;

        //����Ϊ��ֱ��
        if (is_straight1) {
            circle_type = CIRCLE_LEFT_END;
						circle_encoder = current_encoder;
        }
    }
    //�߹�Բ����Ѱ����
    else if (circle_type == CIRCLE_LEFT_END) {
        track_type = TRACK_RIGHT;

        //�����ȶ�����
        if (rpts0s_num < 0.5 / sample_dist) { none_left_line++; }
        if (rpts0s_num > 1.6 / sample_dist && none_left_line > 3&&abs(current_encoder - circle_encoder)>22000) {
            circle_type = CIRCLE_NONE;
						
            none_left_line = 0;
        }
    }
		
    //�һ����ƣ�ǰ��Ѱ��ֱ��
    else if (circle_type == CIRCLE_RIGHT_BEGIN) {
        track_type = TRACK_LEFT;

        //�ȶ����ߺ�����
        if (rpts1s_num < 0.2 / sample_dist) { none_right_line++; }
        if (rpts1s_num > 1.0 / sample_dist && none_right_line > 2) {
            have_right_line++;
            if (have_right_line > 1) {
                circle_type = CIRCLE_RIGHT_IN;
                none_right_line = 0;
                have_right_line = 0;
                circle_encoder = current_encoder;
            }
        }
    }
    //���һ���Ѱ����Բ��
    else if (circle_type == CIRCLE_RIGHT_IN) {
        track_type = TRACK_RIGHT;

        //����������1/4Բ   Ӧ����Ϊ����Ϊת���޹յ�
        if (rpts1s_num < 0.1 / sample_dist ||
            current_encoder - circle_encoder >= ENCODER_PER_METER * (3.14 * 1 / 2)) { circle_type = CIRCLE_RIGHT_RUNNING; }

    }
    //����Ѳ�ߣ�Ѱ��Բ����
    else if (circle_type == CIRCLE_RIGHT_RUNNING) {
        track_type = TRACK_LEFT;

        //�⻷���ڹյ�,���ټӹյ�����о�(��L��)
        if (Lpt0_found) rpts0s_num = rptsc0_num = Lpt0_rpts0s_id;
        if (Lpt0_found && Lpt0_rpts0s_id < 0.4 / sample_dist) {
            circle_type = CIRCLE_RIGHT_OUT;
        }
    }
    //������Ѱ��Բ
    else if (circle_type == CIRCLE_RIGHT_OUT) {
        track_type = TRACK_RIGHT;

        //�󳤶ȼ���б�Ƕ�  Ӧ�����������ҵ���Ϊֱ��
        //if((rpts1s_num >100 && !Lpt1_found))  {have_right_line++;}
        if (is_straight0) {
            circle_type = CIRCLE_RIGHT_END;
        }
    }
        //�߹�Բ����Ѱ����
    else if (circle_type == CIRCLE_RIGHT_END) {
        track_type = TRACK_LEFT;

        //�����ȶ�����
        if (rpts1s_num < 0.2 / sample_dist) { none_right_line++; }
        if (rpts1s_num > 1.0 / sample_dist && none_right_line > 2) {
            circle_type = CIRCLE_NONE;
            none_right_line = 0;
        }
    }
}

// ����Բ��ģʽ�µĵ���ͼ��
void draw_circle() {

}
