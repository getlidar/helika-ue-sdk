# helika-ue-sdk
Helika Unreal Engine SDK

## About Helika
Helika is a leading data analytics, marketing and game management platform designed to empower game studios with the insights needed to make strategic data driven decisions. We provide powerful advanced analytics along with tooling to help teams create stronger marketing (ad/social) campaigns, drive user acquisition, optimize game performance, increase user revenue, and more…all in one tech stack.

Regardless of technical or analytical expertise, studios can double down on what’s working, cut what isn’t, and spend more time building the best games with Helika.

## Unreal Engine SDK
The Helika UE SDK mirrors the functionalities of the Web SDK, catering specifically to UE game developers. This integration facilitates the seamless inclusion of Helika services into your games. By incorporating the Helika UE SDK, you gain effortless access to the complete range of Helika capabilities, empowering you to construct robust and scalable solutions with ease.

Steps to Install Helika Plugin

Step 1: Clone the Plugin Repository
Open a terminal or command prompt.
Navigate to the directory where you want to install the plugin.
Clone the plugin repository using the following command:
git clone https://github.com/getlidar/helika-ue-sdk.git

Step 2: Copy the Plugin to Your Project
Open your Unreal Engine project in the Unreal Editor.
Navigate to the Plugins directory within your project’s folder.
Copy the plugin folder from the cloned repository and paste it into the Plugins directory of your project.

Step 3: Enable the Plugin in Unreal Editor
Launch the Unreal Editor for your project.
Go to the Edit menu and select Plugins.
In the Plugins window, navigate to the Installed section and locate the Helika plugin you installed.
Enable the plugin by checking the checkbox next to its name.
Click the Restart Now button to apply the changes and restart the Unreal Editor.
The plugin is now installed and enabled in your Unreal Engine project.

Steps to Run Helika Events

1. After installing the plugin, Add in the required config values in Project Settings->Plugins->Helika. You'll need to set the Api Key, Game ID, set the Helika Environment to either Localhost, Develop, or Production and set the Telemetry to None, TelemetryOnly or All. You can also programmatically set the Player ID at any time. It is simply appended to all sent events.

![Step1](https://github.com/user-attachments/assets/b2f42773-f694-44f9-b008-fdad38b64b9f)

2. Blueprint Usage : 
    - Access the HelikaManager through HelikaSubsytem and Initialize the SDK. You only have to do this step once (Recommended: Initialize the SDK with your respective GameInstance Init method) 
    ![Step2-1](https://github.com/user-attachments/assets/89014a04-9cae-419e-8e16-0723493a2e6f)

    - Get reference to HelikaManager through HelikaSubsytem and Call "Send Event" Function with your Custom event Data and event name to send event to Helika.
    ![Step2-2](https://github.com/user-attachments/assets/07276684-134d-404a-885c-abe6b1419dac)

    - You can also send an array of events to Helika.
    ![Step2-3](https://github.com/user-attachments/assets/efb028d1-7ced-4590-aefe-49b9c98bb4e4)

3. C++ Usage:
    - Add following dependencies to your project build file and refresh your project.
    ![Step3-1](https://github.com/user-attachments/assets/4bd178e0-f086-4ef9-9990-6045cdec1112)

    - Access the HelikaManager singleton instance and Initialize the SDK. You only have to do this step once (Recommended: Initialize the SDK with your respective GameInstance Init method).
    ![Step3-2](https://github.com/user-attachments/assets/dcb12751-105d-4c6e-9f34-6eea6f5b0e56)

    - Get reference to HelikaManager and Call "Send Event" Function with your Custom event Data and event name to send event to Helika. 
    ![Step3-3](https://github.com/user-attachments/assets/75975fee-5d5f-4781-aa11-207951f56d01)

    - You can also send an array of events to Helika. 
    ![Step3-4](https://github.com/user-attachments/assets/ab4ec2f2-e216-4025-8c8e-33301905c37a)
