# helika-ue-sdk
Helika Unreal Engine SDK

## About Helika
Helika is a leading data analytics, marketing and game management platform designed to empower game studios with the insights needed to make strategic data driven decisions. We provide powerful advanced analytics along with tooling to help teams create stronger marketing (ad/social) campaigns, drive user acquisition, optimize game performance, increase user revenue, and more…all in one tech stack.

Regardless of technical or analytical expertise, studios can double down on what’s working, cut what isn’t, and spend more time building the best games with Helika.

## Unreal Engine SDK
The Helika UE SDK mirrors the functionalities of the Web SDK, catering specifically to UE game developers. This integration facilitates the seamless inclusion of Helika services into your games. By incorporating the Helika UE SDK, you gain effortless access to the complete range of Helika capabilities, empowering you to construct robust and scalable solutions with ease.

Please head to our [docs](https://docs.helika.io/docs/helika-unreal-sdk) for more details.

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

1.After installing the plugin, Add BP_HelikaActor to your level From All->Plugins->Helika Content.

![image](https://github.com/getlidar/helika-ue-sdk/assets/35335528/dba90961-1d64-422a-b35c-91d69dc83507)

2. Now select BP_HelikaActor in your level and add in the required config values in Details Menu.
You'll need to set the Api Key, Game ID and set the Helika Env to either Localhost, Develop, or Production.
You can also programmatically set the Player ID at any time. It is simply appended to all sent events.

![image](https://github.com/getlidar/helika-ue-sdk/assets/35335528/a978b536-3837-480e-91ac-d862438990cc)

3.Get Reference to BP_HelikaActor From your Level and Call "Send Event" Function with you'r Custom event Data to send event to Helika.

![image](https://github.com/getlidar/helika-ue-sdk/assets/35335528/ad962789-65d6-474e-b4db-34707b00740d)


