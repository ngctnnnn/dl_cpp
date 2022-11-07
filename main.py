import os, sys
import argparse 
def main(args):
    
    PATH = os.path.realpath(__file__)[:-(len(os.path.basename(__file__)) + len('/'))]
    
    MASTER_FOLDER_PATH = os.path.realpath(__file__)[:-(len(os.path.basename(__file__)) + len('/'))]
    if not os.path.exists("deeplearning_pytorch/build"):
        os.makedirs("deeplearning_pytorch/build")
    os.system(f"cd deeplearning_pytorch/build && cmake .. > log_file.txt && make -j$nproc > log_file.txt && ./main {args.image_name} {args.image_name[:-4]} {PATH}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--image_name', help='Image name to check')
    args = parser.parse_args()
    main(args)